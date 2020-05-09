#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>

using namespace std;

ofstream out("log.txt", ios::out | ios::trunc);

int completeProducts = 0;

chrono::time_point<chrono::system_clock> start_time;

class resource {
public:
	int a, b, c, d;

	resource(int sum) {
		do {
			a = b = c = d = 0;
			for (int i = 0; i < sum; ++i) {
				srand((chrono::system_clock::now() - start_time).count());
				switch (rand() % 4)
				{
				case 0:
					++a;
					break;
				case 1:
					++b;
					break;
				case 2:
					++c;
					break;
				case 3:
					++d;
					break;
				}
			}
		} while (sum == 5 && !checkValidProductOrder());
	}

	inline bool checkValidProductOrder() {
		return 1 == ((a == 0) + (b == 0) + (c == 0) + (d == 0));
	}

	resource(int a, int b, int c, int d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	bool isEmpty() {
		return !a && !b && !c && !d;
	}

	int getPartTime() {
		return a * 50 + b * 70 + c * 90 + d * 110;
	}

	int getProductTime() {
		return a * 80 + b * 100 + c * 120 + d * 140;
	}

	int getMoveTime() {
		return a * 20 + b * 30 + c * 40 + d * 50;
	}

	resource operator + (resource that) {
		return resource(this->a + that.a, this->b + that.b, this->c + that.c, this->d + that.d);
	}

	resource operator - (resource that) {
		return resource(max(0, this->a - that.a), max(0, this->b - that.b), max(0, this->c - that.c), max(0, this->d - that.d));
	}

	resource operator && (resource that) {
		return resource(min(this->a, that.a), min(this->b, that.b), min(this->c, that.c), min(this->d, that.d));
	}

	resource getPartAvailable(resource capacity, resource current) {
		return (*this) && (capacity - current);
	}

	friend ostream& operator << (ostream& os, const resource& obj);
};

ostream& operator << (ostream& os, const resource& obj) {
	return os << '(' << obj.a << ',' << obj.b << ',' << obj.c << ',' << obj.d << ')';
}

class Plant {
public:
	void receiveMaterials(resource& plan, int workerId, int iteration) {
		{
			chrono::time_point<chrono::system_clock> iteration_start_time = chrono::system_clock::now();

			this_thread::sleep_for(chrono::microseconds(plan.getPartTime()));
			this_thread::sleep_for(chrono::microseconds(plan.getMoveTime()));

			{
				unique_lock<mutex> lg(materialsCountMutex);
				hasCapacity.wait_for(lg, chrono::microseconds(3000), [this, &plan, workerId, iteration, iteration_start_time]() {
					resource order = plan.getPartAvailable(capacity, materialsCount);
					resource remain = plan - order;
					resource materialsAfter = materialsCount + order;

					out << "Current Time: " << chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start_time).count() << "us" << '\n';
					out << "Part Worker ID: " << workerId + 1 << '\n';
					out << "Iteration: " << iteration + 1 << '\n';
					out << "Status: Load Order" << '\n';
					out << "Accumulated Wait Time: " << chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - iteration_start_time).count() << "us" << '\n';
					out << "Buffer State: " << materialsCount << '\n';
					out << "Load Order: " << order << '\n';
					out << "Update Buffer State: " << materialsAfter << '\n';
					out << "Update Load Order: " << remain << '\n';
					out << '\n';

					materialsCount = materialsAfter;
					plan = remain;

					hasMaterials.notify_one();
					return plan.isEmpty();
				});
			}

			this_thread::sleep_for(chrono::microseconds(plan.getMoveTime()));
		}
	}

	void produceProduct(resource& plan, int workerId, int iteration) {
		{
			chrono::time_point<chrono::system_clock> iteration_start_time = chrono::system_clock::now();
			
			resource tmp = plan;

			bool ret = false;
			{
				unique_lock<mutex> lg(materialsCountMutex);
				ret = hasMaterials.wait_for(lg, chrono::microseconds(6000), [this, &plan, workerId, iteration, iteration_start_time]() {
					resource order = materialsCount && plan;
					resource remain = plan - order;
					resource materialsAfter = materialsCount - order;

					out << "Current Time: " << chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start_time).count() << "us" << '\n';
					out << "Product Worker ID: " << workerId + 1 << '\n';
					out << "Iteration: " << iteration + 1 << '\n';
					out << "Status: Pickup Order" << '\n';
					out << "Accumulated Wait Time: " << chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - iteration_start_time).count() << "us" << '\n';
					out << "Buffer State: " << materialsCount << '\n';
					out << "Pickup Order: " << order << '\n';
					out << "Update Buffer State: " << materialsAfter << '\n';
					out << "Update Load Order: " << remain << '\n';
					out << "Total Complete Products: " << completeProducts << '\n';
					out << '\n';

					materialsCount = materialsAfter;
					plan = remain;

					hasCapacity.notify_one();
					return plan.isEmpty();
				});
				completeProducts += ret;
			}

			tmp = tmp - plan;
			this_thread::sleep_for(chrono::microseconds(tmp.getMoveTime()));
			if (ret)
				this_thread::sleep_for(chrono::microseconds(tmp.getProductTime()));
		}
	}

private:
	resource capacity = resource(6, 5, 4, 3);
	resource materialsCount = resource(0, 0, 0, 0);
	mutex materialsCountMutex;
	condition_variable hasMaterials;
	condition_variable hasCapacity;
};

class Producer {
public:
	Producer(Plant& m) : mp{ m } {}

	void deliverMaterials(int workerId, int iteration) {
		resource res(4);
		mp.receiveMaterials(res, workerId, iteration);
	}

private:
	Plant& mp;
};

class Consumer {
public:
	Consumer(Plant& m) : mp{ m } {}

	void getProduct(int workerId, int iteration) {
		resource res(5);
		mp.produceProduct(res, workerId, iteration);
	}

private:
	Plant& mp;
};

int main() {
	start_time = chrono::system_clock::now();

	Plant plant;

	const int nthreads_p = 20;
	const int nthreads_c = 16;

	thread threads_p[nthreads_p];
	thread threads_c[nthreads_c];

	int iteration_per_thread = 5;

	for (int i = 0; i < nthreads_c; ++i) {
		threads_p[i] = (thread)([&plant, i, iteration_per_thread] {
			for (int j = 0; j < iteration_per_thread; ++j) {
				Producer t{ plant };
				t.deliverMaterials(i, j);
			}
		});
		threads_c[i] = (thread)([&plant, i, iteration_per_thread] {
			for (int j = 0; j < iteration_per_thread; ++j) {
				Consumer t{ plant };
				t.getProduct(i, j);
			}
		});
	}

	for (int i = nthreads_c; i < nthreads_p; ++i) {
		threads_p[i] = (thread)([&plant, i, iteration_per_thread] {
			for (int j = 0; j < iteration_per_thread; ++j) {
				Producer t{ plant };
				t.deliverMaterials(i, j);
			}
		});
	}

	for (int i = 0; i < nthreads_c; ++i) {
		threads_p[i].join();
		threads_c[i].join();
	}

	for (int i = nthreads_c; i < nthreads_p; ++i) {
		threads_p[i].join();
	}

	out << "Finish!" << '\n';
	out.close();

	return 0;
}
-- CIS 623 Fall 2020
-- Test 1 Version B

-- Name: Peinan Weng
-- SU-email: peweng@syr.edu

-- I have read all the items listed in the file
-- test1-statement.pdf posted by instructor under
-- our class' blockboard site, and by uploading
-- the answer, I certify that I agree with all
-- of the items in the document.

move :: ([a],[a]) -> ([a],[a])
move (xs,[]) = (xs,[])
move (xs,(y:ys)) = move ((y:xs),ys)

-- Question 1.a
-- move([5,4,3],[2,1])
-- {since [2,1] is not an empty list}
-- = move(2:[5,4,3],[1])
-- = move([2,5,4,3],[1])
-- {since [1] is not an empty list}
-- = move(1:[2,5,4,3],[])
-- = move([1,2,5,4,3],[])
-- {since [] is an empty list}
-- = ([1,2,5,4,3],[])

-- Question 1.b
myFirst :: (a,a) -> a
myFirst (x,y) = x

mySecond :: (a,a) -> a
mySecond (x,y) = y

reverse2 :: [a] -> [a]
reverse2 x = myFirst(move([], x))

--main = print(reverse2([5,4,3,2,1]))

-- Question 2
merge :: Ord a => [a] -> [a] -> [a]
merge [] ys         = ys
merge xs []         = xs
merge (x:xs) (y:ys)
  | x <= y = x : merge xs (y:ys)
  | otherwise = y : merge (x:xs) ys

merge3 :: Ord a => [a] -> [a] -> [a] -> [a]
merge3 xs ys zs = merge(merge xs ys) zs

--merge3_2 :: Ord a => [a] -> [a] -> [a] -> [a]
--merge3_2 xs [] [] = xs
--merge3_2 [] ys [] = ys
--merge3_2 [] [] zs = zs
--merge3_2 xs ys [] = merge xs ys
--merge3_2 xs [] zs = merge xs zs
--merge3_2 [] ys zs = merge ys zs
--merge3_2 (x:xs) (y:ys) (z:zs) 
--  | x <= y && x <= z = x:merge3_2 xs (y:ys) (z:zs)
--  | y <= x && y <= z = y:merge3_2 (x:xs) ys (z:zs)
--  | otherwise = z:merge3_2 (x:xs) (y:ys) zs

--main = print(merge3 [2,4] [1,6] [3,5])

-- Question 3
split :: [Int] -> [([Int],[Int])]
split [] = []
split [x] = []
split (x:xs) = ([x],xs) : [(x:myFirst(t),mySecond(t)) | t <- split(xs)]

--main = print(split [1,3,4,5,8,9])

-- Question 4
bqfour :: [String] -> [String]
bqfour = filter (\x -> x!!0 /= 'a' && x!!0 /= 'A')

--main = print(bqfour ["The","axes","ARE","sharp"])

-- Question 5
ite :: (a -> a) -> a -> Int -> a
ite f x n
  | n <= 0 = x
  | otherwise = f (ite f x (n-1))

--main = print(ite (*2) 5 3)

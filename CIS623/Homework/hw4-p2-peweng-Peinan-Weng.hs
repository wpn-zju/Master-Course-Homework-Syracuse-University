import Data.List

-- CIS 623, Test 3
-- 
-- NAME: Peinan Weng

-- SU-email: peweng@syr.edu

-- Part 1
-- Question a - Question Description
-- Prove double.sum = foldr ((+).double) 0

-- double :: Int -> Int
-- double x = x + x

-- sum :: [Int] -> Int
-- sum [] = 0
-- sum (x:xs) = x + sum xs

-- To prove double.sum = foldr ((+).doublw) 0 is equivalent to the following two equations
-- (double.sum) [] = 0
-- (double.sum) (x:xs) = ((+).double) x ((double.sum) xs)

-- Case []
-- LHS
--   (double.sum) []
-- = { definition of sum }
--   double 0
-- = { definition of double }
--   0

-- RHS
--   0

-- LHS = RHS

-- Case (x:xs)
-- LHS
--   (double.sum) (x:xs)
-- = { composition }
--   double.(sum (x:xs))
-- = { definition of sum }
--   double.(x + sum xs)
-- = { properties of addition & definition of double }
--   x + x + double.sum xs

-- RHS
--   ((+).double) x ((double.sum) xs)
-- = { composition & definition of double }
--   ((+).double) x (double.sum xs)
-- = { composition & definition of double }
--   (+) (x + x) (double.sum xs)
-- = { definition of (+) }
--   x + x + double.sum xs

-- LHS = RHS

-- Question b
-- Prove length.concat = foldr ((+).length) 0

-- length :: [a] -> Int
-- length [] = 0
-- length (x:xs) = 1 + length xs

-- concat :: [[a]] -> [a]
-- concat [] = []
-- concat (xs:xss) = xs ++ concat xss

-- To prove length.concat = foldr ((+).length) 0 is equivalent to the following two equations
-- (length.concat) [] = 0
-- (length.concat) (xs:xss) = ((+).length) xs ((length.concat) xss)

-- Case []
-- LHS
--   (length.concat) []
-- = { definition of concat }
--   length []
-- = { definition of length }
--   0

-- RHS
--   0

-- LHS = RHS

-- Case (xs:xss)
-- LHS
--   (length.concat) (xs:xss)
-- = { composition }
--   length.(concat xs:xss)
-- = { definition of concat }
--   length.(xs ++ concat xss)
-- = { length (a ++ b) = length a + length b, proved in homework }
--   length xs + length.concat xss

-- RHS
--   ((+).length) xs ((length.concat) xss)
-- = { composition }
--   ((+).length) xs (length.concat xss)
-- = { composition & definition of double }
--   (+) (length xs) (length.concat xss)
-- = { definition of (+) }
--   length xs + length.concat xss

-- LHS = RHS

-- Part 2
-- Question a
-- Statement for the mss problem
-- The maximum segment sum problem is to calculate the largest sums of a contiguous segment of a list of integers.

-- Specfication to a solution of the mss problem
-- Solution in Haskell
mss :: [Int] -> Int
mss = maximum.map sum.segments
  where segments = concat.map inits.tails

-- Examples to explain the problem, the given Specfication, its conponenets
-- mss of list [1,-2,5,-4,5,6,-8,7]
-- The result segment should be [5,-4,5,6]
-- We may consider [5,-4,5,6,-8,7] as the result, because the last number 7 is large, however, there is a -8 before 7, making the final sum of [5,-4,5,6,-8,7] less than the sum of [5,-4,5,6]
-- We may also consider [5,6] as the result, because the number -4 before the 5 is small, however, there is a 5 before -4, making the final sum of [5,-4,5,6,-8,7] larger than the sum of [5,6]

-- Question b
mss0 :: [Int] -> Int
mss0 = maximum.map(maximum.scanl (+) 0).tails

-- Solution Description - A short proof is in Question e
-- Step 1 - Generate a tail list of the input list, for example for the given list [1,-2,3] we have
--   [[1,-2,3],[-2,3],[3],[]]
-- Step 2 - For each tail list generated, traverse from head to tail, do 'scanl (+) 0'.
--   [[0,1,-1,2],[0,-2,1],[0,3],[0]]
-- Steo 3 - Get the maximum value in each list.
--   [2,1,3,0]
-- Step 4 - Get the maximum number in the former result as our mss result.
--   3

-- map f.concat = concat.map (map f) - Proof
-- Case []
-- LHS
--   map f.concat []
-- = { definition of map }
--   []

-- RHS
--   concat.map (map f) []
-- = { definition of map }
--   concat []
-- = { definition of concat }
--   []

-- LHS = RHS

-- Case (xs:xss)
-- LHS
--   map f.concat (xs:xss)
-- = { definition of map }
--   (f.concat xs):(map f.concat xss)

-- RHS
--   concat.map (map f) (xs:xss)
-- = { definition of map }
--   concat ((map f xs):(map (map f xss)))
-- = { definition of concat }
--   (map f xs) ++ concat (map (map f xss))
-- = { definition of (+) }
--   

-- maximum.concat = maximum.map maximum - Proof
-- Case []
-- LHS
--   maximum.concat []
-- = { definition of concat }
--   maximum []
-- = { definition of maximum }
--   0

-- RHS
--   maximum.map maximum []
-- = { definition of map }
--   maximum []
-- = { definition of maximum }
--   0

-- LHS = RHS

-- Case (xs:xss)
-- LHS
--   (maximum.concat) (xs:xss)
-- = { definition of concat }
--   maximum (xs ++ concat xss)
-- = { definition of concat }
--   length.(xs ++ concat xss)
-- = { length (a ++ b) = length a + length b, proved in homework }
--   length xs + length.concat xss

-- RHS
--   maximum.map maximum (xs:xss)
-- = { definition of map }
--   maximum ((maximum xs):(map maximum (xs:xss)))
-- = { composition & definition of double }
--   (+) (length xs) (length.concat xss)
-- = { definition of (+) }
--   length xs + length.concat xss

-- Time Complexity O(n^2) Proof
-- mss0 = maximum.map(maximum.scanl (+) 0).tails
-- The map operation on tail lists cost O(n), the scanl operation in each tail list cost O(n), other operations cost O(1).
-- The final Time Complexity is O(n) * O(n) = O(n^2)

-- Question c
-- O(n)
mss1 :: [Int] -> Int
mss1 = maximum.scanr (at) 0
  where at x y = max 0 (x + y)

-- Correctness Proof
-- This function is a combination of 'maximum' and 'scanr'
-- 1. The function 'scanr' will do the 'at' function n times, each time it will take the former result as 'y' and current array element as 'x'. (Also, the first 'y' will be 0.)
-- 2. Let's mark 
--      A. the maximum segment sum of a sub array from a-th element to b-th element as MSS(a, b)
--      B. the maximum segment sum (with the first/a-th element included in the segment) of a sub array from a-th element to b-th element as MSSA(a, b)
--      C. the maximum segment sum (with the last/b-th element included in the segment) of a sub array from a-th element to b-th element as MSSB(a, b)
--    For each step, there will be two cases, consider an input array [a1, a2, a3, a4, ... , ax-1, ax, ax+1, ..., an] and now we are at the x-th element
--    Case 1 - MSSA(x, n) >= 0
--      In this case, MSSB(1, x-1) + MSSA(x, n) (which can be a valid contiguous segment) must be greater than or equal to MSSB(1, x-1).
--      Then we should take MSSA(x, n) into consideration when we are looping from (x-1)-th element back to the 1-st element.
--    Case 2 - MSSA(x, n) < 0
--      In this case, MSSB(1, x-1) + MSSA(x, n) (which can be a valid contiguous segment) must be less than MSSB(1, x-1).
--      Then we should drop all results after the x-th element (include x-th element) and set the current local result to 0.
-- 3. After the loop, the result of scanr will be [MSSA(1, n), MSSA(2, n), ..., MSSA(n-1, n), MSSA(n, n)]
-- 4. The function 'maximum' returns the maximum value in the result of 'scanr', which is the answer we want. (A segment must have a head, so the answer MSS(1, n) must be the maximum element in the array [MSSA(1, n), MSSA(2, n), ..., MSSA(n-1, n), MSSA(n, n)].)

-- Time Complexity O(n) Proof
-- The function scanr has a complexity of O(n * t) (run function t n times) where t is the complexity of inner function (in this case 'at')
-- The complexity of function 'at' is O(1) - It's actually a maximum function + an addition
-- The complexity of function 'maximum' is O(1)
-- Above all, the complexity of function 'mss1' is O(1) * O(n * 1) = O(n)

-- Question d - Alternative O(n) Solution
-- Illustration 
-- 1. Loop from tail to head.
-- 2. Use a pair (Int, Int) to store two integers, one for current best result (first), another for current local result (second).
-- 3. In each step,
--    update the second element of the pair (the current local result) with the min of (x + snd p) and 0. (This step is equivalent to the function 'at' in Question c which can be expand as max (x + y, 0). Here local means this result should have current element included - MSSA in Question c.
--    update the first element of the pair (the current best result) with the min of (x + snd p) and fst p. (This step is equivalent to the final 'maximum' operation in Question c (split the final max/min operation into n max/min operations in each step).
-- 4. The proof is equivalent to the proof of Question c.

-- Solution
f :: Int -> (Int, Int) -> (Int, Int)
f x p = (min (x + snd p) (fst p), min (x + snd p) 0)

v :: (Int, Int)
v = (0, 0)

mss2 :: [Int] -> Int
mss2 xs = fst (foldr f v xs)

-- Question e - Conclusion
-- The maximum segment sum problem has a direct mapping solution with O(n^3) time complexity (given in Question b). However, by doing some optimizations, we can reduce the time complexity to O(n^2) and even O(n).
-- The O(n^3) solution simply generte all potential subarrays of a given list.
-- The O(n^3) to O(n^2) optimization is based on an idea that by traversing a list once, we can get the all MSSA(t, n) (1 <= t <= n) in O(n) time complexity. (The definition of MSSA is in Question c). In this solution, we first calculate the maximum MSSA in every tail list and then get the maximum MSSA in these tail lists as the result of MSS(1, n) of the original list.
-- The O(n^2) to O(n) optimization is based on the idea that we for the full list in the tail lists (the first list element), we already have from MSSA(1, n) to MSSA(n, n). Why do we still calculate from MSSA(2, n) to MSSA(n, n) in the second tail list, calculate from MSSA(3, n) to MSSA(n, n) in the third tail list, etc. By removing those useless tail lists, we can finally get the O(n) solution.

-- main = print (mss1 [1,-2,5,-4,5,6,-8,7])

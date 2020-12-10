isort    :: Ord a => [a] -> [a]
isort []     = [] -- base case
isort (x:xs) = insert x (isort xs)  -- use a helper (local function) insert
    where insert          :: Ord a => a -> [a] -> [a]
          insert x []     = [x]
          insert x (y:ys) 
              | x <= y    = x:y:ys
              | otherwise = y:insert x ys
              
-- evaluation steps of isort[3,2,1]
--   isort[3,2,1]
-- = {since [3,2,1] is not an empty list, isort[3,2,1] = insert 3 (isort[2,1])}
--   insert 3 (isort[2,1])
-- = {since xs([2,1]) is not an empty list, isort[2,1] = insert 2 (isort[1])}
--   insert 3 (insert 2 (isort[1]))
-- = {since xs([1]) is not an empty list, isort[1] = insert 1 (isort[])}
--   insert 3 (insert 2 (insert 1 (isort[])))
-- = {since xs([]) is an empty list, isort[] = []}
--   insert 3 (insert 2 (insert 1 [])
-- = {since ys([]) is an empty list, insert 1 [] = [1]}
--   insert 3 (insert 2 [1])
-- = {since x(2) > y(1), insert 2 [1] = 1:(insert 2 [])}
--   insert 3 (1:(insert 2 []))
-- = {since ys([]) is an empty list, insert 2 [] = [2]}
--   insert 3 (1:[2])
-- = {prepend}
--   insert 3 [1,2]
-- = {since x(3) > y(1), insert 3 [1,2] = 1:(insert 3 [2])}
--   1:(insert 3 [2])
-- = {since x(3) > y(2), insert 3 [2] = 2:(insert 3 [])}
--   1:(2:(insert 3 []))
-- = {since ys([]) is an empty list, insert 3 [] = [3]}
--   1:(2:[3])
-- = {prepend}
--   1:[2,3]
-- = {prepend}
--   [1,2,3]
              
euclid   :: Integer -> Integer -> Integer
euclid x 0 = x
euclid 0 y = y
euclid x y 
    | x <= y = euclid (y - x) x
    | x > y = euclid (x - y) y
    
-- evaluation steps of euclid 6 27
--   euclid 6 27
-- = {since x(6) <= y(27)}
--   euclid 21 6
-- = {since x(21) > y(6)}
--   euclid 15 6
-- = {since x(15) > y(6)}
--   euclid 9 6
-- = {since x(9) > y(6)}
--   euclid 3 6
-- = {since x(3) <= y(6)}
--   euclid 3 3
-- = {since x(3) <= y(3)}
--   euclid 0 3
-- = {since x(0) = 0}
--   3

--main = print(euclid 27 0)
--main = print(euclid 0 27)
--main = print(euclid 6 27)
--main = print(euclid 27 6)
--main = print(euclid 0 0)            
--main = print(isort[3,2,1,6])
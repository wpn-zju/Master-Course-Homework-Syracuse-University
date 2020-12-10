-- Question 1

-- map :: (a -> b) -> [a] -> [b] -- do for each element in the list
-- map f [] = []
-- map f (x:xs) = f x : map f xs

-- reverse :: [a] -> [a] -- reverse a list
-- reverse [] = []
-- reverse (x:xs) = reverse xs ++ [x]

-- LHS
--   map (+2).reverse [1,2,3]
-- = { definition of reverse }
--   map (+2) [3,2,1]
-- = { definition of map }
--   [5,4,3]

-- RHS
--   reverse.map (+2) [1,2,3]
-- = { definition of map }  
--   reverse [3,4,5]
-- = { definition of reverse }  
--   [5,4,3]

-- Question 2
-- zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
-- LHS
--   cols.cols [[1,2],[3,4]]
-- = { rewrite in (xs:xss) format }  
--   cols.cols ([1,2]:[[3,4]])
-- = { definition of cols }
--   cols zipWith (:) [1,2] (cols [[3,4]])
-- = { definition of cols }  
--   cols zipWith (:) [1,2] [[3],[4]]
-- = { definition of zipWith }  
--   cols [1:[3],2:[4]]
-- = { definition of (:) }  
--   cols [[1,3],[2,4]]
-- = { rewrite in (xs:xss) format }
--   cols ([1,3]:[[2,4]])
-- = { definition of cols }
--   zipWith (:) [1,3] (cols [[2,4]])
-- = { definition of cols }  
--   zipWith (:) [1,3] [[2],[4]]
-- = { definition of zipWith }  
--   [1:[2],3:[4]]
-- = { insert }
--   [[1,2],[3,4]]

-- RHS
--   [[1,2],[3,4]]

-- length :: [a] -> Integer
-- length [] = 0
-- length (x:xs) = 1 + length xs

-- reverse :: [a] -> [a] -- reverse a list
-- reverse [] = []
-- reverse (x:xs) = reverse xs ++ [x]

-- proof of length (reverse xs) = length xs
-- case []
-- LHS
--   length (reverse []) 
-- = { definition of reverse }
--   length []
-- = { definition of length }
--   0

-- RHS
--   length []
-- = { definition of length }
--   0

-- case (x:xs)
-- LHS
--   length reverse (x:xs)
-- = { definition of reverse }
--   length (reverse xs ++ [x])
-- = { see proof of length (xs ++ ys) == length xs + length ys below }
--   length (reverse xs) + length [x]
-- = { definition of length }
--   length (reverse xs) + 1 + length []
-- = { definition of length }
--   length (reverse xs) + 1
-- = { inductive hypothesis }
--   1 + length xs

-- RHS
--   length (x:xs)
-- = { definition of length }
--   1 + length xs

-- proof of length (xs ++ ys) == length xs + length ys
-- case []
--   length ([] ++ ys) 
-- = { definition of (++) }
--   length ys
-- = { identity of addition }
--   0 + length ys
-- = { definition of length }
--   length [] + length ys

-- case (x:xs)
--   length ((x:xs) ++ ys)
-- = { definition of (++) }
--   length (x : (xs ++ ys))
-- = { definition of length }
--   1 + length (xs ++ ys)
-- = { inductive hypothesis }
--   1 + length xs + length ys
-- = { associativity assumption }
--   (1 + length xs) + length ys
-- = { definition of length }
--   (length (x:xs)) + length ys

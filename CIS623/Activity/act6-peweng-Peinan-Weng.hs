-- hide these functions in Prelude
import Prelude hiding (zip,map,concat,replicate)

-- zip using recursion
zip :: [a] -> [b] -> [(a,b)]
-- recursively add pairs to the result
zip (x:xs) (y:ys) = (x,y):zip xs ys
-- if either xs or ys is an empty list, return an empty list.
zip xs ys = []

-- map using list comprehension
map :: (a -> b) -> [a] -> [b]
-- for each x in xs, do f for x and adding to the result list.
map f xs = [f x | x <- xs]

-- concat using list comprehension
concat :: [[a]] -> [a]
-- for each xs in xss and each x in xs (drop the outer list), add all elements to the result list.
concat xss = [x | xs <- xss, x <- xs]

-- replicate using list comprehension
replicate :: Int -> a -> [a]
replicate k x
    | k <= 0 = [] -- if k <= 0, then return an empty list.
    | otherwise = [x | _ <- [1..k]] -- otherwise, add x to the result list, repeat k times. There are k elements in [1..k], so the action will repeat k times.
    
-- dotproduct using zipWith
dotproduct :: Num a => [a] -> [a] -> a
-- zip the two input lists xs and ys with multiplication(*) and them sum up as the result.
dotproduct xs ys = sum (zipWith (*) xs ys)

main = print(zip [1,2,3,4,5] [5,4,3,2,1])
--main = print(map reverse [[1,2,3,4]])
--main = print(concat [[1,2,3,4],[4,2,4]])
--main = print(replicate 5 [1,2,3])
--main = print(dotproduct [1.1, 2.3, 4.5] [2.0, 1.0])
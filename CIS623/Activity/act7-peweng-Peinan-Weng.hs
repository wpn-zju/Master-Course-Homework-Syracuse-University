import Prelude hiding (takeWhile,dropWhile,until)
import Data.Char (isLower)

example1a :: [String] -> [String]
example1a strs = map (\ s -> s ++ ['.']) strs

example1b :: [String] -> [String]
example1b strs = filter (\ s -> isLower (head s)) strs

takeWhile :: (a -> Bool) -> [a] -> [a]
takeWhile p [] = []
takeWhile p (x:xs)
  | p x = x : takeWhile p xs
  | otherwise = []

dropWhile :: (a -> Bool) -> [a] -> [a]
dropWhile p [] = []
dropWhile p (x:xs)
  | p x = dropWhile p xs
  | otherwise = (x:xs)

until :: (a -> Bool) -> (a -> a) -> a -> a
until p f x = if p x then x else until p f (f x)

-- until (>50) (*5) 1
-- = {expand}
-- if (>50) 1 then 1 else until (>50) (*5) ((*5) 1)
-- = {since (>50) 1 = false}
-- until (>50) (*5) ((*5) 1)
-- = {calculate}
-- until (>50) (*5) 5
-- = {expand}
-- if (>50) 5 then 5 else until (>50) (*5) ((*5) 5)
-- = {since (>50) 5 = false}
-- until (>50) (*5) ((*5) 5)
-- = {calculate}
-- until (>50) (*5) 25
-- = {expand}
-- if (>50) 25 then 25 else until (>50) (*5) ((*5) 25)
-- = {since (>50) 25 = false}
-- until (>50) (*5) ((*5) 25)
-- = {calculate}
-- until (>50) (*5) 125
-- = {expand}
-- if (>50) 125 then 125 else until (>50) (*5) ((*5) 125)
-- = {since (>50) 125 = true}
-- 125

--main = print (example1a ["th", "ewr", "dfsa"])
--main = print (example1b ["Rewr", "dfsa", "213f", ".fdf", "ssw"])
--main = print (takeWhile (>5) [1,2,4,3,6,54,3,4,5,67])
--main = print (dropWhile(>5) [1,2,4,3,6,54,3,4,5,67])
main = print (until (>50) (*5) 1)
-- CIS 623, Test 2, Version A
-- 
-- NAME: Peinan Weng

-- SU-email: peweng@syr.edu

-- Acknowledgement:

-- By submitting this file, I acknowledge that I accept the
-- six items as stated in the acknowledgement section in the 
-- beginning of the test 2 document.


-----------------------------------------------------------------

-- For Question 1, 2 

data Shape = Circle Float | Rect Float Float

-----------------------------------------------------------------

-- Answer to question 1 a:

-- q1eg1
q1eg1 :: Shape
q1eg1 = Circle 1.0

-- q1eg2
q1eg2 :: Shape
q1eg2 = Rect 2.0 3.0

-- Answer to question 1 b:

-- show function for Shape class
instance Show Shape where
  show (Circle r) = "(c rad.= " ++ show r ++ ")"
  show (Rect l w) = "(r l= " ++ show l ++ " w= " ++ show w ++ ")"

--main = print q1eg1
--main = print q1eg2

-- Answer to question 2:

enlargeShape :: Float -> Shape -> Shape
enlargeShape f (Circle r) = (Circle (r * f))
enlargeShape f (Rect l w) = (Rect (l * f) (w * f))

-- function enlarge
enlarge :: Float -> [Shape] -> [Shape]
enlarge f xs = [enlargeShape f x | x <- xs]

--main = print (enlarge 2.0 [q1eg1, q1eg2])

-----------------------------------------------------------------

-- For Question 3 

data Formula = Var Char
             | Not Formula
             | And Formula Formula 
             | Or Formula Formula 
             | Imply Formula Formula 

-- question 3 helpers

f1 :: Formula
f1 = And (Var 'A') (Not (Var 'B'))

f2 :: Formula
f2 = Imply (And (Var 'A') (Var 'B')) (Var 'A')

f3 :: Formula
f3 = Imply (Var 'A') (And (Var 'A') (Var 'B'))

f4 :: Formula
f4 = Not (Or (Not (Or (Var 'A') (Or (Var 'A') (Var 'B')))) (Var 'B'))

instance Show Formula where
  show (Var b) = show b
  show (Not p) = "!" ++ show p
  show (And p q) = "(" ++ show p ++ " & " ++ show q ++ ")"
  show (Or p q) = "(" ++ show p ++ " | " ++ show q ++ ")"
  show (Imply p q) = "(" ++ show p ++ " -> " ++ show q ++ ")"

-- Answer to question 3:

-- function transform
transform :: Formula -> Formula -- Implication Free
transform (Var b) = Var b
transform (Not p) = Not (transform p)
transform (And p q) = And (transform p) (transform q)
--transform (Or p q) = transform (Not (And (Not p) (Not q)))
--transform (Imply p q) = transform (Not (And p (Not q)))
transform (Or p q) = Not (And (Not (transform p)) (Not (transform q)))
transform (Imply p q) = Not (And (transform p) (Not (transform q)))

--main = print f4
--main = print (transform f4)

-----------------------------------------------------------------

-- For Question 4

data RTree a = Vertex a [RTree a]
               deriving Show

-- Answer to question 4 a

-- q4eg1 
q4eg1 :: RTree Int
q4eg1 = Vertex 1 []

-- q4eg2 
q4eg2 :: RTree Int
q4eg2 = Vertex 2 [Vertex 3 []]

-- q4eg3 
q4eg3 :: RTree Int
q4eg3 = Vertex 4 [(Vertex 5 []), (Vertex 6[])]

-- Answer to question 4 b

-- function relabel
relabel :: (a -> b) -> RTree a -> RTree b
relabel f (Vertex rt cl) = Vertex (f rt) [relabel f c | c <- cl]

--main = print (relabel show q4eg1)
--main = print (relabel show q4eg2)
--main = print (relabel show q4eg3)

-----------------------------------------------------------------

-- Answer to question 5

-- function h 
h :: (Eq a) => a -> a -> Int -> Int
h countnum x result
  | x == countnum = result + 1
  | otherwise = result

-- value z
z :: Int
z = 0

-- count function given, uncomment below to test function count
count :: (Eq a) => a -> [a] -> Int
count x xs = foldr (h x) z xs

--main = print (count 1 [1,2,3,4,1,1,1])

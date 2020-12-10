-- CIS 623, final

-- version A
-- 
-- NAME: Peinan Weng

-- SU-email:  peweng@syr.edu

-- Acknowledgement:

-- By submitting this file (my answers to final examination, part 2), 
-- I acknowledge that I accept the terms as stated in the rules section 
-- of the examination document.


---------------------------------------------------

import Data.List

-- Question 1  (20 point)


-- Code for composer1
composer1 :: [a -> a] -> a -> a
composer1 [] v = v
composer1 (f:fs) v = f (composer1 fs v)



-- Code for composer2
composer2 :: [a -> a] -> a -> a
composer2 [] v = v
composer2 (f:fs) v = composer2 fs (f v)

-- main = print(composer2 [(*2),(+3)] 0)


---------------------------------------------------
-- End of Question 1
---------------------------------------------------

-- Question 2

-- Code for firstpos
firstpos :: (Eq a) => (a -> a) -> (a -> a) -> [a] -> Int
firstpos f g [] = -1
firstpos f g xs = head [y |  (x, y) <- (zip xs [0..]), (f x) /= (g x)] 

main = print (firstpos (^2) (*2) [0,2,4,6])



---------------------------------------------------
-- End of Question 2
---------------------------------------------------


-- Question 3



-- 1.
data BTree a = Nil 
            | Vertex a (BTree a) (BTree a)

-- t0 is:
t0 :: BTree a
t0 = Nil

-- t1 is:
t1 :: BTree Int
t1 = Vertex 1 t2 t3

-- t2 is:
t2 :: BTree Int
t2 = Vertex 2 Nil Nil

-- t3 is:
t3 :: BTree Int
t3 = Vertex 3 Nil Nil


-- 2

-- code for numinrange
numinrange :: (Ord a) => a -> a -> BTree a -> Int
numinrange a b Nil = 0
numinrange a b (Vertex v cl cr) 
                        | (v >= a && v <= b) = 1 + (numinrange a b cl) + (numinrange a b cr)
                        | otherwise = (numinrange a b cl) + (numinrange a b cr)

--main = print (numinrange 1 3 t1)

---------------------------------------------------
-- End of Question 3
---------------------------------------------------


-- Question 4


-- fun is:
fun :: Int -> (Int, Int) -> (Int, Int)
fun x r
    | x >= fst r = (x, fst r)
    | x >= snd r = (fst r, x)
    | otherwise = r

-- v is :
v :: (Int, Int)
v = (0, 0)

-- seclargest is given as:
seclargest    :: [Int] -> Int
seclargest xs = snd (foldr fun v xs)

-- Note: uncomment above 2 lines to test run

--main = print (seclargest [1])

---------------------------------------------------
-- End of Question 4
---------------------------------------------------

-- Question 5


-- Code for eval
data Nat = Zero 
        | Succ Nat
        deriving Show
        
data Exp a = Plus (Exp a) (Exp a)
        | Times (Exp a) (Exp a) 
        | Val a
        deriving Show
        
toNumber :: Nat -> Int
toNumber Zero = 0
toNumber (Succ t) = 1 + toNumber t

eval :: Exp Nat -> Int
eval (Plus t1 t2) = eval t1 + eval t2
eval (Times t1 t2) = eval t1 * eval t2
eval (Val t) = toNumber t

--main = print (eval (Plus (Val (Succ (Succ Zero))) (Val (Succ (Succ (Succ Zero))))))



---------------------------------------------------
-- End of Question 5
---------------------------------------------------


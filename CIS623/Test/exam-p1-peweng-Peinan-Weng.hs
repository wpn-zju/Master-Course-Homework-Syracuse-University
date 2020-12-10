import Prelude hiding (map, concat)

-- Question 1
-- This equation is FALSE

-- We can easily write a counterexample.
-- Assume 
--   a = 10
--   b = 0
--   input = [0]
-- LHS
--   ((max 10).foldr min 0) [0]
-- = { composition }
--   max 10 (foldr min 0 [0])
-- = { definition of min and foldr }
--   max 10 0
-- = { definition of max }
--   10
-- RHS
--   foldr min (max 0 10) [0]
-- = { definition of max }
--   foldr min 10 [0]
-- = { definition of min and foldr }
--   0

-- LHS != RHS

-- Then, if we want to use the fusion property of fold to prove this equation. The h, g, w, f and v are as follows.
-- h = max a
-- g = min
-- w = b
-- f = min
-- v = max a b

-- In this case
-- h w = v => max a b = max a b
-- h (g x y) = f x (h y) => max a (min x y) = min x (max a y)
-- The first one is true.
-- The second one is obviously false. The order of min and max will definitely affect the result. (The left expression is at least a, however, the right expression is at most x, so if a is greater than x, this equation can't be valid.)
-- So we can't prove this equation based on the fusion property of fold.

-- Question 2
-- Definition of map
map :: (a -> b) -> [a] -> [b]
map f [] = []
map f (x:xs) = f x:map f xs

-- Definition of concat
concat :: [[a]] -> [a]
concat [] = []
concat (xs:xss) = xs ++ concat xss

-- input :: [[a]]
-- output :: [b]

--map f.concat = concat.map (map f)

-- Case 1 - []
-- LHS
--   (map f.concat) []
-- = { composition }
--   map f (concat [])
-- = { definition of concat }
--   map f []
-- = { definition of map }
--   []

-- RHS
--   (concat.map (map f)) []
-- = { composition }
--   concat (map (map f)) [])
-- = { definition of map }
--   concat []
-- = { definition of concat }
--   []

-- LHS = RHS

-- Case 2 - (xs:xss)
-- LHS
--   (map f . concat) (xs:xss)
-- = { composition }
--   map f (concat xs:xss)
-- = { definition of concat }
--   map f (xs ++ concat xss)
-- = { property of map, map f (x ++ y) = (map f x) ++ (map f y) }
--   (map f xs) ++ (map f (concat xss))
-- = { composition }
--   (map f xs) ++ ((map f . concat) xss)
-- = { inductive hypothesis }
--   (map f xs) ++ ((concat . map(map f)) xss)
-- = { composition }
--   (map f xs) ++ (concat (map (map f) xss))

-- RHS
--   (concat . map (map f)) (xs:xss)
-- = { composition }
--   concat (map (map f) (xs:xss))
-- = { definition of map }
--   concat ((map f xs):(map (map f) xss))
-- = { definition of concat }
--   (map f xs) ++ (concat (map (map f) xss))

-- LHS = RHS

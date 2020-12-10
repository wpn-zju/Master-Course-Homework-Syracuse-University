import Prelude hiding (sqrt)

-- The function leq (Integer -> Float -> Bool) will
-- return True - if m <= x
-- retrun False - if m > x
leq :: Integer -> Float -> Bool
m `leq` x = fromInteger m <= x

-- The function lt (Float -> Integer -> Bool) will
-- return True - if x < n
-- return Flase - if x >= n
lt :: Float -> Integer -> Bool
x `lt` n = x < fromInteger n

-- The function until (a -> Bool) -> (a -> a) -> a -> a
-- The type of the first argument is (a -> Bool), which means a function accepting an a and return a Bool value. This argument is used to check if the value meets the requirement to return as the final result.
-- The tyoe of the second argument is (a -> a), which means a function accepting an a and return a an as well. This argument is used to update the third argument in a recursive call.
-- The type of the third argument is a. This is the base input of the until function.
-- The until function will keep checking if the third argument meets the requirement of the first argument function, if yes, return, otherwise, update the third argument using the second argument function.

geq :: Integer -> Float -> Bool
m `geq` x = fromInteger m >= x

gt :: Float -> Integer -> Bool
x `gt` n = x > fromInteger n

smallestodd :: Float -> Integer
smallestodd x = if x > 1
            then until (`geq` x) (+2) 1
            else until ( x `gt`) (subtract 2) 1 + 2
            
floor2 :: Float -> Integer
floor2 x = fst (until unit (shrink x) (bound x))
           where unit (m,n) = (m + 1 == n)
           
type Interval = (Integer,Integer)

shrink :: Float -> Interval -> Interval
shrink x (m,n) = if p `leq` x then (p,n) else (m,p)
                 where p = choose (m,n)

choose :: Interval -> Integer
choose (m,n) = (m+n) `div` 2

bound :: Float -> Interval
bound x = (lower x, upper x)

lower :: Float -> Integer
lower x = until (`leq` x) (*2) (-1)

upper :: Float -> Integer
upper x = until (x `lt`) (*2) 1

-- Evaluation step of floor2 3.29
--   floor2 3.29
-- = {expand}
--   fst (until unit (shrink 3.29) (bound 3.29))
-- = {calculate bound 3.29}
--   fst (until unit (shrink 3.29) (-1, 4))
-- = {since -1 + 1 != 4}
--   fst (until unit (shrink 3.29) shrink 3.29 (-1, 4))
-- = {since choose (-1, 4) = 1 and is less than 3.29}
--   fst (until unit (shrink 3.29) (1, 4))
-- = {since 1 + 1 != 4}
--   fst (until unit (shrink 3.29) shrink 3.29 (1, 4))
-- = {since choose (1, 4) = 2 and is less than 3.29}
--   fst (until unit (shrink 3.29) (2, 4))
-- = {since 2 + 1 != 4}
--   fst (until unit (shrink 3.29) shrink 3.29 (2, 4))
-- = {since choose (2, 4) = 3 and is less than 3.29}
--   fst (until unit (shrink 3.29) (3, 4))
-- = {since 3 + 1 == 4}
--   fst (3, 4)
-- = {get the first element}
--   3

-- The function iterate (a -> a) -> a -> [a]
-- The function iterate will generate an infinite list with the second argument be the first element of the list. In that infinite list, any element (except the first one) will be f(x), where the f is the first argument function and x is the previous element in the list.

sqrt :: Float -> Float
sqrt x = until goodenough improve x
  where goodenough y = abs (y*y-x) < eps*x
        improve y = (y+x/y)/2
        eps = 0.000001
        
improve :: Float -> Float -> Float
improve x y = (y+x/y)/2
        
check :: Float -> Float -> Bool
check x y = abs (y*y-x) < eps*x
    where eps = 0.000001

main = print(head(filter(check 100) (iterate (improve 100) 100)))
import Prelude hiding (take, init)
import Data.List hiding (take, init)

take :: Int -> [a] -> [a]
take n [] = [] -- If the input list is empty, then return an empty list.
take n (x:xs) 
    | n > 0 = x : take (n-1) xs -- Only if n is positive, do take. Return the first n elements recursively.
    | otherwise = [] -- Otherwise, return an empty list. This make sure we could handle non-positive n inputs.
    
--main = print(take (-2) [1,2,3,4,5])

-- When the input is a infinity list, the function null will return False.

--main = print(null [1..])

init :: [b] -> [b]
init [] = [] -- If the input list is empty, then return an empty list.
init [x] = [] -- If the input list only has one element, then drop it and return an empty list.
init (x:xs) = x : init xs -- If not, do init until x comes to the last element.

--main = print(init [1,2,3,4])

--main = print(inits [1,2,3,4,5])
-- result = [[],[1],[1,2],[1,2,3],[1,2,3,4],[1,2,3,4,5]]
--main = print(inits "0123")
-- result = ["","0","01","012","0123"]

-- What inits do? inits returns the list of initial segements of its argument list, shortest first.
-- If the input list is L, the return value is [[],..., init(init(init L)), init(init L), init L, L]
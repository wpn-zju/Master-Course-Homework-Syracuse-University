merge :: Ord a => [a] -> [a] -> [a]
merge [] ys         = ys
merge xs []         = xs
merge (x:xs) (y:ys)
  | x <= y = x : merge xs (y:ys)
  | otherwise = y : merge (x:xs) ys
  
halve :: [a] -> ([a],[a])
halve x = (take middleIndex x, drop middleIndex x)
    where middleIndex = div (length x) 2

msort [] = []
msort [x] = [x]
msort x = msort (fst p) `merge` msort (snd p)
    where p = halve x

main = print(msort[1,5,2,6,4,8,2,9,4,3,7])
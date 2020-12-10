import Data.Char

let2int :: Char -> Int
let2int c = ord c - ord 'a'

int2let :: Int -> Char
int2let n = chr (ord 'a' + n)

-- Add let2intU for uppercase letters.
let2intU :: Char -> Int
let2intU c = ord c - ord 'A'

-- Add int2letU for uppercase letters
int2letU :: Int -> Char
int2letU n = chr (ord 'A' + n)

shift :: Int -> Char -> Char
shift n c
  | isLower c = int2let ((let2int c + n) `mod` 26)
  | isUpper c = int2letU ((let2intU c + n) `mod` 26) -- Here add another case for uppercase letters.
  | otherwise = c
    
encode :: Int -> String -> String
encode n xs = [shift n x | x <- xs]

decode :: Int -> String -> String
decode n = encode (-n)

table :: [Float]
table = [   8.1, 1.5, 2.8, 4.2, 12.7, 2.2, 2.0, 6.1, 7.0,
            0.2, 0.8, 4.0, 2.4, 6.7, 7.5, 1.9, 0.1, 6.0,
            6.3, 9.0, 2.8, 1.0, 2.4, 0.2, 2.0, 0.1]
            
percent :: Int -> Int -> Float
percent n m = (fromIntegral n / fromIntegral m) * 100

lowers :: String -> Int
lowers xs = length [x | x <- xs, x >= 'a' && x <= 'z']

-- Add uppers to count the total number of upper case letters
uppers :: String -> Int
uppers xs = length [x | x <- xs, x >= 'A' && x <= 'Z']

count :: Char -> String -> Int
count x xs = length [x' | x' <- xs, x == toLower x'] -- Here, add a toLower function to count uppercase and lowercase letter at the same time.

positions :: Eq a => a -> [a] -> [Int]
positions x xs = [i | (x', i) <- zip xs [0..], x == x']

freqs :: String -> [Float]
freqs xs = [percent (count x xs) n | x <- ['a'..'z']]
    where n = lowers xs + uppers xs -- Here, in the counting part, change it to lowers xs + uppers xs.
    
chisqr :: [Float] -> [Float] -> Float
chisqr os es = sum [((o-e)^2)/e | (o,e) <- zip os es]

rotate :: Int -> [a] -> [a]
rotate n xs = drop n xs ++ take n xs

crack :: String -> String
crack xs = encode (-factor) xs
    where
        factor = head (positions (minimum chitab) chitab)
        chitab = [chisqr (rotate n table') table | n <- [0..25]]
        table' = freqs xs

main = print(crack "hello WORLD!")
-- Basic elements used to generate number strings
units, teens, tens :: [String]
-- Basic number from 0 to 9
units = ["zero","one","two","three","four","five",
         "six","seven","eight","nine"]
-- Special cases 'teen's from 10 - 19
teens = ["ten","eleven","twelve","thirteen","fourteen",
         "fifteen","sixteen","seventeen","eighteen",
         "nineteen"]
-- 'ty's from 20 to 90
tens  = ["twenty","thirty","forty","fifty","sixty",
         "seventy","eighty","ninety"]

-- Get String element.
convert1 :: Int -> String
convert1 n = units!!n

-- Get Div by 10 result and Mod by 10 result as a (Int, Int) pair.
digits2 :: Int -> (Int,Int)
digits2 n = n `divMod` 10 -- equals to divMod n 10

-- First call digits2 to generate a (Int, Int) pair, and then pass it to combine2 to generate a String.
convert2 :: Int -> String
convert2 = combine2 . digits2

-- Converting number less than 100
combine2 :: (Int,Int) -> String
combine2 (t,u)
   | t==0         = units!!u -- If the 10 digit is 0, return a certain element in 'units' list.
   | t==1         = teens!!u -- If the 10 digit is 1, return a certain element in 'teens' list.
   | 2<=t && u==0 = tens!!(t-2) -- If the 10 digit is greater than 1 and the 1 digit is 0, simply return a certain element in 'tens' list.
   | 2<=t && u/=0 = tens!!(t-2) ++ "-" ++ units!!u -- If the 10 digit is greater than 1 and the 1 digit is not 0, return a 'tens' string appended with '-' and a 'unit' string.

-- Get Div by 100 result and Mod by 100 result as a (Int, Int) pair.
digits3 :: Int -> (Int,Int)
digits3 n = n `divMod` 100 -- equals to divMod n 100

-- First call digits3 to generate a (Int, Int) pair, and then pass it to combine3 to generate a String.
convert3 :: Int -> String
convert3 = combine3 . digits3

-- Converting number less than 1,000
combine3 :: (Int,Int) -> String
combine3 (h,n)
   | h==0      = convert2 n -- If the 100 digit is 0, simply return the result of convert2 n.
   | n==0      = units!!h ++ " hundred" -- If the 100 digit is positive but the rest part is 0, simply return a 'unit' string appended with ' hunderd'.
   | otherwise = units!!h ++ " hundred and " ++ convert2 n -- If the 100 digit is positive and the rest part is also positive, return a 'unit' string appended with ' hundred and ' and convert2 n.

-- Get Div by 1000 result and Mod by 1000 result as a (Int, Int) pair.
digits6 :: Int -> (Int,Int)
digits6 n = n `divMod` 1000

-- First call digits6 to generate a (Int, Int) pair, and then pass it to combine6 to generate a String.
convert6 :: Int -> String
convert6 = combine6 . digits6

-- Converting number less than 1,000,000
combine6 :: (Int,Int) -> String
combine6 (m,n)
  | m==0      = convert3 n -- If the 1,000 digit is 0, then simply return the result of convert3 n.
  | n==0      = convert3 m ++ " thousand" -- If the 1,000 digit is not 0 but the rest part is 0, return the result of convert3 m appended with ' thousand'.
  | otherwise = convert3 m ++ " thousand" ++ link n ++
                convert3 n -- Otherwise, return the result of convert3 m (how many thousands) appended with ' thousand' and convert3 n (number in 1,000). We also need this 'link n' to handle special cases.

-- Link different parts of the number string. This function is used to deal with special cases like '999099'.
link :: Int -> String
link n = if n < 100 then " and " else " "

-- convert = convert6
convert :: Int -> String
convert = convert6

--main = print (convert1 3)
--main = print (digits2 30)
--main = print (combine2 (1,2))
main = print (convert 999099)
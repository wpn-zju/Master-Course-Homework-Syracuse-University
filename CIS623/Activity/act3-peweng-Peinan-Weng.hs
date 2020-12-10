-- Acitivity 3

import Data.Char -- (toLower)
import Data.List -- (sort,words)
import System.Environment -- (getArgs)

-- Type Synonym
type Key = [Char]

addKey :: String -> (Key, String)
addKey w = (sort w, w)

-- The type signature of function is (Key, String) -> [(Key, [String])] -> [(Key, [String])].
-- Insert a (Key, String) pair to a [(Key, [String])] list.
myInsert :: (Key, String) -> [(Key, [String])] -> [(Key, [String])]
-- If the current result pair list is empty, simply insert the (k, w) as (k, [w]).
myInsert (k, w) [] = [(k, [w])]
-- If the current result pair list is not empty, then compare the sorted word key k with the key of the first element in the result list, which is k'.
myInsert (k, w) ((k', ws):kws) = 
    if k == k' 
        -- If the key k in (k, w) pair equals to the key of the first pair in the result list (which is k'), then we insert our new word w at the front of the value of that pair, which is a list of String called ws.
        then (k, w:ws):kws
        -- Otherwise, we add a new [(Key, [String])] pair (as (k, [w])) for the word we want to insert, because the sorted key is different with every keys in the previous result list.
        else (k, [w]):(k', ws):kws

-- Group the anagram
groupByKey :: [(Key, String)] -> [(Key, [String])]
groupByKey [] = []
-- For each key-word pair (Key, String), insert it into the result list.
groupByKey (kw:kws) = myInsert kw (groupByKey kws)

-- Three Examples
--groupByKey(map addKey ["grgh", "rggh", "ghhr", "hghr"]))
--groupByKey(map addKey ["faer", "reaf", "redede", "eddeer"]))
--groupByKey(map addKey ["faer", "qweqw", "weqwq", "sraf"]))

-- display 10 unicode symbols from 40 to 49
showUnicode :: IO ()
showUnicode = putStrLn([chr(40)..chr(49)])

--main = showUnicode

-- myReplicate function using recursion
myReplicate :: Int -> a -> [a]
myReplicate 0 x = [ ]
myReplicate n x = x : myReplicate (n - 1) x

--main = print (myReplicate 10 1)
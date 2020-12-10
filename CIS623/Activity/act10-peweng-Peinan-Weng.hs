-- Review of Question 3
-- Section 1 - Introduction
-- Write a Haskell function split which returns all splittings of a list into two non-empty parts that append to give the original list. For example, split [1,2,3,4] should give: [([1],[2,3,4]),([1,2],[3,4]),([1,2,3],[4])].

-- My Solution
-- "cons" operator + list comprehension
-- split :: [Int] -> [([Int],[Int])]
-- split [] = []
-- split [x] = []
-- split (x:xs) = ([x],xs) : [(x:fst(t),snd(t)) | t <- split(xs)]

-- Section 2 - Attempts
-- This function was to expand a list, so I came up with the recursion using the "cons" operator in Haskell. But when I thought deeper, I found this problem could be much more complex than the recursive function tasks in our daily activities.

-- Then I decided to write each step of split [1,2,3,4] to get a better understanding of this problem.

-- First of all, when we are using the "cons" operator, we always extract the first element from the list and combine (do something with this element) with (do something with the rest part of the list) as the return value. In other words, when we are using this operator, we want to convert the function from f(x:xs) to a combination of g(x) and h(xs).

-- Here we have split [1,2,3,4] = [([1],[2,3,4]),([1,2],[3,4]),([1,2,3],[4])]. 
-- What if we drop the first element "1" from the result? Then it will be "[],[2,3,4]", "[2],[3,4]" and "[2,3],[4]".

-- Notice that:
-- "[2,3,4]" is the "xs" of the first call.
-- "[2],[3,4]" together with "[2,3],[4]" is the result of split [2,3,4], we can write a simple solution now.

-- split (x:xs) = ([x],xs) : (Something do with the result of split(xs))

-- Then we have split [2,3,4] = [([2],[3,4]),([2,3],[4])].
-- split (x:xs) = ([x],xs) : [(x:fst(t),snd(t)) | t <- split(xs)]

-- Section 3 - Remarks & Conclusion
-- When we are writing Haskell recursive functions, it may be hard to write down the recursive expression directly. A better way is to write a example and do the function step by step.
-- For list functions, we often use the operator "cons" for recursion. In other programming languages, it's common that we use iteration like a for loop to write those list functions. But in Haskell, the code logic can be quite different. When the function becomes more complex, we'd better write some examples to make the problem more clear and some key logic will emerge in our analysis.
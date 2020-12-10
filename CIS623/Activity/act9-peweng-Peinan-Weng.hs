type Row a = [a]
type Matrix a = [Row a]
type Digit = Char
type Choices = [Digit]
type Grid = Matrix Digit

-- smallest
-- Returns the k-th element of the merge result of 2 input lists.
smallest :: Ord a => Int -> ([a],[a]) -> a
smallest k (xs,ys) = union(xs,ys)!!k

-- union
-- Merge 2 lists
union :: Ord a => ([a],[a]) -> [a]
union (xs,[]) = xs
union ([],ys) = ys
union (x:xs,y:ys)
    | x <= y = x : union(xs,y:ys)
    | otherwise = y : union(x:xs,ys)
    
-- digits
-- Define valid digits
digits :: [Digit]
digits = ['1'..'9']

-- blank
-- Define blank
blank :: Digit -> Bool
blank = (== '0')

-- 1. Specification
-- solve1
-- Step 1 - Generate all choices for each digit in the sudoku.
-- Step 2 - Expand. Generate all combinations of those choices we calculated in Step 1.
-- Step 3 - Check the validity of each potential choices combination, return those valid combinations as the result.
solve1 :: Grid -> [Grid]
solve1 = filter valid.expand.choices

-- choice
-- Genearte choice for one digit in the sudoku.
choice :: Digit -> [Digit]
choice d
    | blank d = digits
    | otherwise = [d]

-- choices
-- Convert every digits to its choice => generate a matrix of (a list of potential digits).
choices :: Grid -> Matrix Choices
choices = map (map choice)
-- Alt: choices xss = [[choice x | x <- xs] | xs <- xss]

-- expand
-- Generate all combinations of the choices.
-- The first step 'map cp xs' or '[cp x | x <- xs]' generates combinations for each row in the choices matrix and save as a list. Each element in the list indicates all combinations of choices for a certain row in the original matrix.
-- The second step 'cp' generates all combinations for the whole sudoku matrix based on the result of the first step.
expand :: Matrix Choices -> [Grid]
expand = cp.(map cp)
-- Alt: expand xs = cp [cp x | x <- xs]

-- cp (Cartesian Product)
-- Generate all combinations
cp :: [[a]] -> [[a]]
cp [] = [[]]
cp (xs:xss) = [x:ys | x <- xs, ys <- cp xss]

-- Example 1
--   cp [[1,2,3]]
-- = cp [1,2,3]:[]
-- = [x:ys | x <- [1,2,3], ys <- cp []]
-- = [x:ys | x <- [1,2,3], ys <- []]
-- = [[1],[2],[3]]

-- Example 2
--   cp [[1],[2],[3]]
-- = cp [1]:[[2],[3]]
-- = [x:ys | x <- [1], ys <- cp [[2],[3]]]
-- = [x:ys | x <- [1], ys <- cp [2]:[[3]]]
-- = [x:ys | x <- [1], ys <- [x:ys | x <- [2], ys <- cp [[3]]]
-- = [x:ys | x <- [1], ys <- [x:ys | x <- [2], ys <- cp [3]:[]]]
-- = [x:ys | x <- [1], ys <- [x:ys | x <- [2], ys <- [x:ys | x <- [3], ys <- cp []]]]
-- = [x:ys | x <- [1], ys <- [x:ys | x <- [2], ys <- [x:ys | x <- [3], ys <- []]]]
-- = [x:ys | x <- [1], ys <- [x:ys | x <- [2], ys <- [[3]]]]
-- = [x:ys | x <- [1], ys <- [2,3]]
-- = [[1,2,3]]

-- Example 3
--   cp [[1,2,3],[4,5,6],[7,8,9]]
-- = cp [1,2,3]:[[4,5,6],[7,8,9]]
-- = [x:ys | x <- [1,2,3], ys <- cp [[4,5,6],[7,8,9]]]
-- = [x:ys | x <- [1,2,3], ys <- cp [4,5,6]:[[7,8,9]]]
-- = [x:ys | x <- [1,2,3], ys <- [x:ys | x <- [4,5,6], ys <- cp [[7,8,9]]]
-- = [x:ys | x <- [1,2,3], ys <- [x:ys | x <- [4,5,6], ys <- cp [7,8,9]:[]]]
-- = [x:ys | x <- [1,2,3], ys <- [x:ys | x <- [4,5,6], ys <- [x:ys | x <- [7,8,9], ys <- cp []]]]
-- = [x:ys | x <- [1,2,3], ys <- [x:ys | x <- [4,5,6], ys <- [x:ys | x <- [7,8,9], ys <- []]]]
-- = [x:ys | x <- [1,2,3], ys <- [x:ys | x <- [4,5,6], ys <- [[7],[8],[9]]]]
-- = [x:ys | x <- [1,2,3], ys <- [[4,7],[4,8],[4,9],[5,7],[5,8],[5,9],[6,7],[6,8],[6,9]]]
-- = [
--     [1,4,7],[1,4,8],[1,4,9],[1,5,7],[1,5,8],[1,5,9],[1,6,7],[1,6,8],[1,6,9],
--     [2,4,7],[2,4,8],[2,4,9],[2,5,7],[2,5,8],[2,5,9],[2,6,7],[2,6,8],[2,6,9],
--     [3,4,7],[3,4,8],[3,4,9],[3,5,7],[3,5,8],[3,5,9],[3,6,7],[3,6,8],[3,6,9]
--   ]

--main = print (cp [[1,2,3]])
--main = print (cp [[1],[2],[3]])
--main = print (cp [[1,2,3],[4,5,6],[7,8,9]])

-- valid
-- Check if the grid is a valid solution.
valid :: Grid -> Bool
-- Check all rows / cols / boxes
valid g = all nodups (rows g) && all nodups (cols g) && all nodups (boxs g)

-- nodups
-- Check if there is a repeated digit in the list.
nodups :: Eq a => [a] -> Bool
nodups [] = True
nodups (x:xs) = x `notElem` xs && nodups xs

-- rows
-- Generate a list of rows
rows :: Matrix a -> [Row a]
rows = id -- no need to change the original input

-- cols
-- Generate a list of cols
cols :: Matrix a -> [Row a]
cols [xs] = [[x] | x <- xs]
cols (xs:xss) = zipWith (:) xs (cols xss)

-- boxs
-- Generate a list of 9-element boxes
boxs :: Matrix a -> [Row a]
boxs = (map ungroup).ungroup.(map cols).group.(map group)
-- boxs ["123","456","789"]
-- map ungroup.ungroup.map cols.group(map group ["123","456","789"])
-- map ungroup.ungroup.map cols.group [["123","456","789"]]
-- map ungroup.ungroup.map cols [["123","456","789"]]
-- map ungroup.ungroup.map [["123"],["456"],["789"]]

-- ungroup
-- Simply call concat to ungroup 3-element groups into one list.
ungroup :: [[a]] -> [a]
ungroup = concat

-- group
-- Convert a list to 3-element group lists.
group :: [a] -> [[a]]
group [] = []
group (x:y:z:xs) = [x,y,z]:group xs

-- Example for boxs function
-- boxs [[1,2,3,4,5,6],[1,2,3,4,5,6],[1,2,3,4,5,6]]
-- = (map ungroup).ungroup.(map cols).group.(map group [[1,2,3,4,5,6],[1,2,3,4,5,6],[1,2,3,4,5,6]])
-- = (map ungroup).ungroup.(map cols).group [[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]]]
-- = (map ungroup).ungroup.(map cols [[[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]]]])
-- = (map ungroup).ungroup [[[[1,2,3],[1,2,3],[1,2,3]],[[4,5,6],[4,5,6],[4,5,6]]]]
-- = map ungroup [[[1,2,3],[1,2,3],[1,2,3]],[[4,5,6],[4,5,6],[4,5,6]]]
-- = [[1,2,3,1,2,3,1,2,3],[4,5,6,4,5,6,4,5,6]]

--main = print(map group [[1,2,3,4,5,6],[1,2,3,4,5,6],[1,2,3,4,5,6]])
--main = print (group [[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]]])
--main = print (map cols [[[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]]]])
--main = print (ungroup [[[[1,2,3],[1,2,3],[1,2,3]],[[4,5,6],[4,5,6],[4,5,6]]]])
--main = print (map ungroup [[[1,2,3],[1,2,3],[1,2,3]],[[4,5,6],[4,5,6],[4,5,6]]])
--main = print (boxs [[1,2,3],[4,5,6],[7,8,9]])

-- 2. Pruning
-- prune - prune by rows then prune by cols then prune by boxs
-- pruneBy rows x = rows.(map pruneRow (rows x))
-- = rows [pruneRow row | row <- rows]
-- first do pruneRow for each row in the rows.
-- since rows.rows x = x, the left rows is used to revert the choice matrix.
-- pruneBy cols x = cols.(map pruneRow (cols x))
-- = cols [pruneRow col | col <- cols]
-- first do pruneRow for each col in the rows.
-- since cols.cols x = x, the left cols is used to revert the choice matrix.
-- pruneBy boxs x = boxs.(map pruneRow (boxs x))
-- = boxs [pruneRow box | box <- boxs]
-- first do pruneRow for each box in the rows.
-- since boxs.boxs x = x, the left boxs is used to revert the choice matrix.
prune :: Matrix Choices -> Matrix Choices
prune = (pruneBy boxs).(pruneBy cols).(pruneBy rows) -- Prune by boxs/cols/rows.
    where pruneBy f = f.(map pruneRow).f

-- pruneRow
-- How to prune? - Remove those determined digits (singleton) in choice lists of each undetermined digit (non-singleton) in the row.
-- For example
-- digit 1 has choices 1,2,3,4 and 5,
-- digit 2 (in the same row) and digit 3 (in the same row) are determined with 2 and 4,
-- then the choice list of digit 1 should be pruned to 1,3 and 5.
pruneRow :: Row Choices -> Row Choices
pruneRow row = map (remove ones) row -- remove those digits which are already filled in other digits in the same row/col/box.
    where ones = [d | [d] <- row] -- collect those determined digits in a row/col/box.

-- remove
remove :: Choices -> Choices -> Choices
remove xs [d] = [d] -- Skip if the digit is already a determined digit.
remove xs ds = filter (`notElem` xs) ds -- Otherwise, remove those determined digits in the choice list of the current digit because they can not be used by the current digit.

-- 3. Single-cell expansion
-- expand1
-- Expand the pruned grid with undetermined digits.
expand1 :: Matrix Choices -> [Matrix Choices]
expand1 rows = [rows1 ++ [row1 ++ [c]:row2] ++ rows2 | c <- cs]
    where
        (rows1,row:rows2) = break(any smallest) rows -- Deal with rows of choice lists - [[[Digit]]]
        -- 5. break from a row where contains a shortest choice list.
        (row1,cs:row2) = break smallest row -- Deal with one row of choice lists - [[Digit]]
        -- 6. break from the shortest choice list.
        smallest cs = length cs == n
        -- 4. If a choice list has a length of the minimum ,then it's the smallest.
        n = minimum (counts rows)
        -- 3. Calculate the minimum length of the choice list, expand starting from the shortest choice list.
        counts = filter (/=1).(map length).concat -- Calculate the number of potential grids.
        -- 1. Concat all rows into one list
        -- 2. For each choice list in the big list generated before, filter those choice lists with more than 1 choice in it.

-- 4. Final algorithm
-- solve2
-- Step 1 - Generate all choices for each digit in the sudoku.
-- Step 2 - See search.
solve2 :: Grid -> [Grid]
solve2 = search.choices

-- search
search :: Matrix Choices -> [Grid]
search cm
    | not (safe pm) = [] -- Check if the grid has no solutions.
    | complete pm = [map (map head) pm] -- If the pruned grid with choice lists is already a completed grid, remove the choice list (every choice lists only has 1 element) and return the grid as result.
    | otherwise = (concat.(map search).expand1) pm -- Otherwise, we have to expand the current grid, generate all possible combinations and add them all in a grid list as the result.
    -- Expand -> Search -> Add.
    where pm = prune cm -- Prune the choices list of each digit in the grid.
    
-- complete
-- Check if all digits in the grid are determined.
complete :: Matrix Choices -> Bool
complete = all (all single)

-- single
-- Check if the given argument is a single-element list (determined).
single [_] = True
single _ = False

-- safe
-- Returning false means the grid is contradictory.
safe :: Matrix Choices -> Bool
safe cm = all ok (rows cm) && all ok (cols cm) && all ok (boxs cm)

-- ok
-- Check if those determined digits in each row/col/box are contradictory.
-- In other words, are there repeated determined digits in a row/col/box?
ok :: Row Choices -> Bool
ok row = nodups [d | [d] <- row]

main = print(solve2 [
    "534678910",
    "672195348",
    "198342560",
    "859761423",
    "426853791",
    "700024856",
    "961537284",
    "287419635",
    "345286179"])
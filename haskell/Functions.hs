--lambda
--one argument
my_pow = (\x-> x * x)
--two arguments
my_multiply = (\x y -> x * y)

f:: Num a => a -> a
f x = 4 * x +1

area r = pi *r^2

infixl 5 <@>
(<@>) :: [a] -> Int -> a
(<@>) xs i = last $ take (i + 1) xs

fac :: Int -> Int
fac 0 = 1
fac 1 = 1
fac x = if x < 0 then error "n < 0"
	else x * fac (x-1)

snoc :: a -> [a] -> [a]
snoc i [] = [i]
snoc i (x:xs) = x : snoc i xs

last' :: [a] -> a
last' [] = error "empty list"
last' [x] = x
last' (_:xs) = last' xs

take' :: Int -> [a] -> [a]
take' 0 _ = []
take' i [] = if i > 0 then error "index out of bounds"
	     else []
take' i (x:xs) = x : take' (i - 1) xs


take'' :: Int -> [a] -> [a]
take'' n _ | n <= 0 = []
take'' _ [] = []
take'' n (x:xs) = x : take'' (n-1) xs


head' :: [a] -> a
head' [] = error "empty list"
head' (x:xs) = x

elem' :: Eq a => a -> [a] -> Bool
elem' _ [] = False
elem' e (x:xs) = x == e || elem' e xs

delete :: Int -> [a] -> [a]
delete n [] | n > 0 = error "index out of bounds"
delete 0 (x:xs) = xs
delete n (x:xs) = x : delete (n-1) xs

drop' :: Int -> [a] -> [a]
drop' 0 xs = xs
drop' n [] | n > 0 = error "index out of bounds"
drop' n (x:xs) = drop (n-1) xs

{-
function call pattern match test
tt 1 [] -> Exception 0 -- branch zero
tt 0 [] -> Exception 1 -- branch zero match fail, fall to branch one
tt 1 [3] -> Non-exhaustive patterns in function tt -- all branch not macth, throw an exception
-}
tt :: Int -> [a] -> [a]
tt n [] | n > 0 = error "0"
tt 0 [] = error "1"

total' :: [Int] -> Int
total' [] = 0
total' (x:xs) = x + total' xs

_total :: [Int] -> Int -> Int
_total [] sum = sum
_total (x:xs) sum = _total xs $! (x + sum)

total'' :: [Int] -> Int
total'' xs = _total xs 0

-- always returns 91
macCarthy :: Int -> Int
macCarthy n | n > 100 = n - 10
            | otherwise = macCarthy $ macCarthy (n + 11)

fibo :: (Num a, Eq a) => a -> a
fibo 0 = 1
fibo 1 = 1
fibo n = fibo (n - 1) + fibo (n - 2)

{- fastFibo -}
fiboStep :: Num a => (a, a) -> (a, a)
fiboStep (u, v) = (v, u + v)

fiboPair :: (Num a, Eq a) => a -> (a, a)
fiboPair 0 = (0, 1)
fiboPair n = fiboStep $ fiboPair (n - 1)

fastFibo :: (Num a, Eq a) => a -> a
fastFibo n = snd $ fiboPair n

golden :: Fractional a => Int -> [a]
golden n = take n (map (\(x, y) -> x/y) (iterate fiboStep (0, 1)))
{- end fastFibo -}

--todo bug here
_binSearch :: Ord a =>  Int -> Int -> a -> [a] -> Int
_binSearch left right key xs
    | left > right = -1
    | otherwise = let mid = (left + right) `div` 2 in
        let m =  xs!!mid in
        if m == key then mid
            else if m < key
                then _binSearch (mid + 1) right key xs
                else _binSearch left (mid - 1) key xs

mySearch :: Ord a => a -> [a] -> Int
mySearch key xs = _binSearch 0 (length xs) key xs

------------binary search---------------
binSearch :: Ord a => a -> [a] -> Bool
binSearch a [] = False
binSearch a xs | m < a = binSearch a behid
               | m > a = binSearch a front
               | otherwise = True
                 where (front, m:behid) = splitAt (length xs `div` 2) xs

----------insert sort------------------
insert :: Ord a => a -> [a] -> [a]
insert a [] = [a]
insert a (x:xs) | a < x = a:x:xs
                | otherwise = x : insert a xs

{--the grecious implementation--}
insertSort :: Ord a => [a] -> [a]
insertSort [] = []
insertSort (x:xs) = insert x (insertSort xs)


{-- dumb one-}
_myInsertSort :: Ord a => {--store--} [a] {--input--} -> [a] -> [a]
_myInsertSort store [] = store
_myInsertSort store (x:xs) = _myInsertSort (insert x store) xs

{--add explict function type here to avoid monomorphism issue--}
myInsertSort :: Ord a => [a] -> [a]
myInsertSort = _myInsertSort []
{-- end dump one-}

------------bubble sort (fixed point)------------
{--myBubbleSort the dumb way--}

--Crap!! I cannot evencome up with the dumb way

{--end the dumb way--}

swaps :: Ord a => [a] -> [a]
swaps [] = []
swaps [x] = [x]
swaps (x1:x2:xs) | x1 > x2 = x2 : swaps (x1:xs)
                 | otherwise = x1 : swaps (x2:xs)

fix :: Eq a => (a->a) -> a -> a
fix f x = if x == x' then x else fix f x'
             where x' = f x

bubbleSort :: Ord a => [a] -> [a]
bubbleSort xs = fix swaps xs

--my
myBubbleSort :: Ord a => [a] -> [a]
myBubbleSort xs = if xs == xs' then xs else myBubbleSort xs'
                    where xs' = swaps xs

--in the book
{-- fuck i don't know why, but the compiler complains about indent
bubbleSort' :: Ord a => [a] -> [a]
bubbleSort' xs | swaps xs == xs = xs
               | otherwise bubbleSort' $ swaps xs
--}


bubbleSort'' :: Ord a => [a] -> [a]
bubbleSort'' [] = []
bubbleSort'' xs = bubbleSort'' initialElements ++ [lastElement]
                    where swappedxs = swaps xs
                          initialElements = init swappedxs
                          lastElement = last swappedxs


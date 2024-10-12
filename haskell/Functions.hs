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
{- end fastFibo -}

golden :: Fractional a => Int -> [a]
golden n = take n (map (\(x, y) -> x/y) (iterate fiboStep (0, 1)))

--binary search
binSearch :: Ord a => a -> [a] -> Bool
binSearch a [] = False
binSearch a xs | m < a = search a behid
               | m > a = search a front
               | otherwise = True
                 where (front, m:behid) = splitAt (length xs `div` 2) xs

"""Required questions for lab 2"""

## Boolean Operators ##

# Q4
def both_positive(x, y):
    """Returns True if both x and y are positive.

    >>> both_positive(-1, 1)
    False
    >>> both_positive(1, 1)
    True
    """
    "*** YOUR CODE HERE ***"
    return (x > 0) and (y > 0)


## while Loops ##

# Q7
def factors(n):
    """Prints out all of the numbers that divide `n` evenly.

    >>> factors(20)
    20
    10
    5
    4
    2
    1
    """
    "*** YOUR CODE HERE ***"
    for num in range(1, n+1): 
        if n % num == 0: 
           print(n//num)


# Q8
def fib(n):
    """Returns the nth Fibonacci number.

    >>> fib(0)
    0
    >>> fib(1)
    1
    >>> fib(2)
    1
    >>> fib(3)
    2
    >>> fib(4)
    3
    >>> fib(5)
    5
    >>> fib(6)
    8
    """
    "*** YOUR CODE HERE ***"
    f0 = 0 
    f1 = 1 
    if n == 0: 
       return 0
    if n == 1:
       return 1
    count = 1 
    while(count < n): 
        temp = f0
        f0 = f1
        f1 = temp + f1
        count = count + 1 
    return f1     

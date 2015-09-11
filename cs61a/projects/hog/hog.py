"""The Game of Hog."""


from dice import four_sided, six_sided, make_test_dice

from ucb import main, trace, log_current_line, interact


GOAL_SCORE = 100 # The goal of Hog is to score 100 points.


######################

# Phase 1: Simulator #

######################


def roll_dice(num_rolls, dice=six_sided):

    # These assert statements ensure that num_rolls is a positive integer.

    assert type(num_rolls) == int, 'num_rolls must be an integer.'

    assert num_rolls > 0, 'Must roll at least once.'

    # BEGIN Question 1

    points, i, out = 0, 0, False

    while i < num_rolls:

        a = dice()

        points, i = points + a, i+1

        if a == 1:

            out = 1

    return out or points

    # END Question 1



def take_turn(num_rolls, opponent_score, dice=six_sided):

    """Simulate a turn rolling NUM_ROLLS dice, which may be 0 (Free bacon).


    num_rolls:       The number of dice rolls that will be made.

    opponent_score:  The total score of the opponent.

    dice:            A function of no args that returns an integer outcome.

    """

    assert type(num_rolls) == int, 'num_rolls must be an integer.'

    assert num_rolls >= 0, 'Cannot roll a negative number of dice.'

    assert num_rolls <= 10, 'Cannot roll more than 10 dice.'

    assert opponent_score < 100, 'The game should be over.'

    # BEGIN Question 2

    oppscr = max(opponent_score // 10, opponent_score % 10)

    if num_rolls == 0:

        points = oppscr + 1

    else:

        points = roll_dice(num_rolls, dice)

    return points

    # END Question 2


def select_dice(score, opponent_score):

    """Select six-sided dice unless the sum of SCORE and OPPONENT_SCORE is a

    multiple of 7, in which case select four-sided dice (Hog wild).

    """

    # BEGIN Question 3

    if (score + opponent_score) % 7 == 0:
        return four_sided
    else:
        return six_sided

    # END Question 3


def is_swap(score0, score1):

    """Return True if ending a turn with SCORE0 and SCORE1 will result in a

    swap.


    Swaps occur when the last two digits of the first score are the reverse

    of the last two digits of the second score.

    """

    # BEGIN Question 4
    
    temp0 = score0 % 100
    temp1 = score1 % 100
    bool1 = (temp0 % 10) == (temp1//10)
    bool2 = (temp0//10) == (temp1 % 10)
    return bool1 and bool2 

    # END Question 4



def other(who):

    """Return the other player, for a player WHO numbered 0 or 1.


    >>> other(0)



    >>> other(1)



    """

    return 1 - who

def wins(score0, score1, goal):
    if score0 >= goal or score1 >= goal:
        return True
    else:
        return False
def play(strategy0, strategy1, score0=0, score1=0, goal=GOAL_SCORE):

    """Simulate a game and return the final scores of both players, with

    Player 0's score first, and Player 1's score second.


    A strategy is a function that takes two total scores as arguments

    (the current player's score, and the opponent's score), and returns a

    number of dice that the current player will roll this turn.


    strategy0:  The strategy function for Player 0, who plays first

    strategy1:  The strategy function for Player 1, who plays second

    score0   :  The starting score for Player 0

    score1   :  The starting score for Player 1

    """

    who = 0  # Which player is about to take a turn, 0 (first) or 1 (second)

    # BEGIN Question 5
    
    while True:
        score0 = score0 + take_turn(strategy0(score0, score1), score1, select_dice(score0, score1))
    
        if is_swap(score0, score1):
            temp = score0
            score0 = score1
            score1 = temp 
            if wins(score0, score1, goal):
                return score0, score1
            score1 = score1 + take_turn(strategy1(score1,score0), score0, select_dice(score0, score1))
            if is_swap(score0, score1):
                temp = score0
                score0 = score1
                score1 = temp 

        else:
            if wins(score0, score1, goal):
                return score0, score1
            score1 = score1 + take_turn(strategy1(score1,score0), score0, select_dice(score0, score1))
            if is_swap(score0, score1):
                temp = score0
                score0 = score1
                score1 = temp 


        if wins(score0,score1, goal):
            return score0, score1

        
    
    
    

    # END Question 5

    return score0, score1


#######################

# Phase 2: Strategies #

#######################


def always_roll(n):

    """Return a strategy that always rolls N dice.


    A strategy is a function that takes two total scores as arguments

    (the current player's score, and the opponent's score), and returns a

    number of dice that the current player will roll this turn.


    >>> strategy = always_roll(5)

    >>> strategy(0, 0)



    >>> strategy(99, 99)



    """

    def strategy(score, opponent_score):

        return n

    return strategy


# Experiments


def make_averaged(fn, num_samples=1000):

    """Return a function that returns the average_value of FN when called.


    To implement this function, you will have to use *args syntax, a new Python

    feature introduced in this project.  See the project description.


    >>> dice = make_test_dice(3, 1, 5, 6)

    >>> averaged_dice = make_averaged(dice, 1000)

    >>> averaged_dice()


    >>> make_averaged(roll_dice, 1000)(2, dice)



    In this last example, two different turn scenarios are averaged.

    - In the first, the player rolls a 3 then a 1, receiving a score of 1.

    - In the other, the player rolls a 5 and 6, scoring 11.

    Thus, the average value is 6.0.

    """

    # BEGIN Question 6
    
    def h(*args):
        sum = 0 
        count = 1 
        
        while count <= num_samples:
            sum += fn(*args)
        
            count = count +1  
        return (sum/num_samples)
    return h 
    # END Question 6


def max_scoring_num_rolls(dice=six_sided, num_samples=1000):

    """Return the number of dice (1 to 10) that gives the highest average turn

    score by calling roll_dice with the provided DICE over NUM_SAMPLES times.

    Assume that dice always return positive outcomes.


    >>> dice = make_test_dice(3)

    >>> max_scoring_num_rolls(dice)



    """

    # BEGIN Question 7

    maximum = 0 

    for x in range(1,11):
        current = make_averaged(roll_dice, num_samples)(x, dice)
        if current > maximum:
            answer = x 
            maximum = current
    return answer
    # END Question 7


def winner(strategy0, strategy1):

    """Return 0 if strategy0 wins against strategy1, and 1 otherwise."""

    score0, score1 = play(strategy0, strategy1)

    if score0 > score1:

        return 0

    else:

        return 1


def average_win_rate(strategy, baseline=always_roll(5)):

    """Return the average win rate (0 to 1) of STRATEGY against BASELINE."""

    win_rate_as_player_0 = 1 - make_averaged(winner)(strategy, baseline)

    win_rate_as_player_1 = make_averaged(winner)(baseline, strategy)

    return (win_rate_as_player_0 + win_rate_as_player_1) / 2 # Average results


def run_experiments():

    """Run a series of strategy experiments and report results."""

    if True: # Change to False when done finding max_scoring_num_rolls

        six_sided_max = max_scoring_num_rolls(six_sided)

        print('Max scoring num rolls for six-sided dice:', six_sided_max)

        four_sided_max = max_scoring_num_rolls(four_sided)

        print('Max scoring num rolls for four-sided dice:', four_sided_max)


    if False: # Change to True to test always_roll(8)

        print('always_roll(8) win rate:', average_win_rate(always_roll(8)))


    if False: # Change to True to test bacon_strategy

        print('bacon_strategy win rate:', average_win_rate(bacon_strategy))


    if False: # Change to True to test swap_strategy

        print('swap_strategy win rate:', average_win_rate(swap_strategy))



    "*** You may add additional experiments as you wish ***"


# Strategies


def bacon_strategy(score, opponent_score, margin=8, num_rolls=5):

    """This strategy rolls 0 dice if that gives at least MARGIN points,

    and rolls NUM_ROLLS otherwise.

    """

    # BEGIN Question 8

    if max(opponent_score // 10, opponent_score % 10) + 1 >= margin:
        return 0
    else:
        return num_rolls

    

    # END Question 8


def swap_strategy(score, opponent_score, margin=8, num_rolls=5):

    """This strategy rolls 0 dice when it results in a beneficial swap and

    rolls NUM_ROLLS if rolling 0 dice results in a harmful swap. It also

    rolls 0 dice if that gives at least MARGIN points and rolls NUM_ROLLS

    otherwise.

    """

    # BEGIN Question 9

    point = max(opponent_score // 10, opponent_score % 10) + 1
    if score + point == opponent_score:
        return bacon_strategy(score, opponent_score, margin, num_rolls)
    if is_swap(score + point, opponent_score):
        if score + point < opponent_score:
            return 0
        elif score + point > opponent_score:
            return num_rolls
    else:
        return bacon_strategy(score, opponent_score, margin, num_rolls)
    

    # END Question 9



def final_strategy(score, opponent_score):

    """Write a brief description of your final strategy.


    If I can force the opponent to roll the 4 dice, then I will, since the expected value is half of the 6-sided die.
    Then when I am losing by 50, I will start getting desperate and take bigger risks, like rolling 10. 
    If we are playing in a normal position, then I will just play to get optimal expected value. 

    """

    # BEGIN Question 10
    if select_dice(score + take_turn(0, opponent_score), opponent_score) == four_sided:
        return 0
    if opponent_score - score > 50:
        return 10
    if score + opponent_score % 7 == 0:
        return swap_strategy(score, opponent_score, 4.4, 4 )
    else: 
        return swap_strategy(score, opponent_score, 8.7, 6 )    

    
    
    # END Question 10



##########################

# Command Line Interface #

##########################


# Note: Functions in this section do not need to be changed.  They use features

#       of Python not yet covered in the course.



@main

def run(*args):

    """Read in the command-line argument and calls corresponding functions.


    This function uses Python syntax/techniques not yet covered in this course.

    """

    import argparse

    parser = argparse.ArgumentParser(description="Play Hog")

    parser.add_argument('--final', action='store_true',

                        help='Display the final_strategy win rate against always_roll(5)')

    parser.add_argument('--run_experiments', '-r', action='store_true',

                        help='Runs strategy experiments')

    args = parser.parse_args()


    if args.run_experiments:

        run_experiments()

    elif args.final:

        from hog_eval import final_win_rate

        win_rate = final_win_rate()

        print('Your final_strategy win rate is')

        print('    ', win_rate)

        print('(or {}%)'.format(round(win_rate * 100, 2)))
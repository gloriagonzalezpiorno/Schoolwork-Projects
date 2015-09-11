# CS 61A World Game Data:
from classes import *

# Characters:

robert = Character('Robert', "I can't believe I lost my wallet again! "
                             "I wish someone could find it for me.")
albert = Character('Albert', 'Robert went to Soda for potluck. You can find him there.')
yulin = Character('Yulin', 'Are you going to 61A potluck? '
                           'You should bring some board games!')
jeffrey = Character('Jeffrey', 'No one brought food to the potluck! '
                               'Maybe the Golden Bear Cafe (GBC) is open; we can get food there.')
derrick = Character('Derrick', 'I heard you like board games. '
                               'Have you gone to Games of Berkeley on Shattuck?')
ken = Character('Ken', 'Hey! Want to play ultimate frisbee?')
student = Character('Student', 'I once went into Dwinelle and got lost for 3 days! '
                               'That place is a maze!')


# Things:
wallet = Thing('Wallet', "Looks like Robert's wallet. We should return it to him.")
hotdog = Thing('Hotdog', 'Yummy! Bring it to 61A potluck!')
cards = Thing('Monopoly', 'Just right for 61A potluck!')


# Places:

sather_gate = Place('Sather Gate', 'You are at Sather Gate', [], [])
fsm = Place('FSM', 'You are at Free Speech Cafe', [], [wallet])
vlsb = Place('VLSB', 'You are at VLSB', [albert], [])
soda = Place('Soda', 'You are at Soda', [robert, jeffrey], [])
gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [hotdog])
campanile = Place('Campanile', 'You are at Campanile', [yulin], [])
game_store = Place('Games of Berkeley', 'You are at Games of Berkeley', [], [cards])
woz = Place('Woz', 'You are at Wozniak Lounge', [], [])
shattuck = Place('Shattuck', 'You are at Shattuck', [], [])
wheeler = Place('Wheeler Hall', 'You are at Wheeler Hall', [derrick], [])
dwinelle = Place('Dwinelle Hall', 'You are at Dwinelle Hall', [student], [])
memorial_glade = Place('Memorial Glade', 'You are at Memorial Glade', [ken], [])


# Exits:
sather_gate.add_exits([gbc, wheeler, dwinelle, memorial_glade])
gbc.add_exits([sather_gate])
wheeler.add_exits([sather_gate, campanile])
dwinelle.add_exits([sather_gate, vlsb])
memorial_glade.add_exits([sather_gate, fsm, campanile, soda])
campanile.add_exits([memorial_glade, wheeler])
vlsb.add_exits([fsm, soda, shattuck, dwinelle])
shattuck.add_exits([vlsb, game_store])
fsm.add_exits([vlsb, memorial_glade])
soda.add_exits([woz, vlsb, memorial_glade])
woz.add_exits([soda])
game_store.add_exits([shattuck])

# Player:
# The Player should start at sather_gate.
me = Player("Xingyou", sather_gate)


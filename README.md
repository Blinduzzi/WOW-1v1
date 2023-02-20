# WOW-1v1
C++ project for my OOP uni course

You have to implement a simplified version of a Player-versus-Player interaction from World of Warcraft. The implementation involves 
sequential 1v1 fights between the own character and another character.

A fight can be described through several rounds; at the beginning of each round, each Character chooses an ability to use during that round:

● for the own Character, the ability is read from the keyboard;

● for the enemy Character, the ability is automatically chosen by the computer.

A Character is described as follows:

● Name

● Level (LEVEL)

● Base Attributes

● Health (BASE_HP)

● Attack (BASE_ATK)

● Defense (BASE_DEF)

● Special Attack (BASE_SP_ATK)

● Special Defense (BASE_SP_DEF)

● Speed (BASE_SPD)

At the beginning of the fight, the level-specific attributes are calculated for both players using the formulas:

● Health (HP_LEVEL) = [(BASE_HP * LEVEL) / 50] + LEVEL + 10

● Any other attribute (ATR_LEVEL) = [(BASE_ATR * LEVEL) / 50] + 10

Except for the actual health attribute (HP), any other effective attribute (ATR) can be modified in the fight by the abilities used. 
The value of effective attributes is calculated using the level-specific attributes and a modification level (STAGE), with the following properties:

● The modification level (STAGE) is specific to each attribute;

● STAGE is an integer between -6 and 6;

● If STAGE is positive, then the effective value ATR = [(2 + STAGE) * ATR_LEVEL / 2]

● If STAGE is negative, then the effective value ATR = [2 * ATR_LEVEL / (2 - STAGE)]

The actual health attribute (HP) of a player decreases every time they take damage (DMG) and cannot be increased during a fight. 
At the beginning of each fight, HP is equal to the level-specific attribute (HP_LEVEL) for both players involved.

During each round, the one with the highest effective speed (SPD) goes first and chooses an ability. If there is a tie in effective 
speeds, the own character goes first.

When players use abilities that cause damage (DMG), the effective health (HP) of the opponent decreases by the inflicted damage (DMG).
The first character to reach an effective health level less than or equal to 0 loses the fight.

If the own character wins the fight, then they receive experience points (EXP) equal to the level of the defeated character (LEVEL). 
If their current experience (EXP) is at least double their level (2 * LEVEL), they will advance to the next level and lose that value 
(2 * LEVEL) from their experience (EXP). A player's character can advance more than one level after a fight. Characters cannot have a level 
(LEVEL) greater than 100.

If the own character loses the fight, they will not receive experience (EXP). Character abilities have at least one of the following effects:

● Cause damage (DMG) with a certain power (PWR) of a certain type (TYPE_ATK) using the formula:

DMG = [(LEVEL * PWR * TYPE_ATK) / (60 * ENEMY_TYPE_DEF)]

TYPE_ATK can be ATK or SP_ATK. The enemy is forced to defend with the corresponding type.

● Modify an own attribute (ATR) with a certain modification level (STAGE), which can also be negative.

● Modify an attribute for the enemy (ATR) with a certain modification level (STAGE), which can also be negative.

Skills can have at most one damage-causing effect (DMG), but can have multiple attribute modification effects (MOD) 
(either for oneself, for the enemy, or mixed). The damage-causing effect (DMG) is always applied first, and only then are 
the attribute modifications applied, if the fight has not already been won.

Characters know a list of abilities and can only use those abilities. Characters have one of the following 4 control modes:

● Easy: automatically; uses only the first known ability;

● Medium: automatically; uses all abilities in order starting from the first, and when finished starts again from the beginning;

● Advanced: automatically; in each round uses the ability that causes the most damage (DMG).

● Custom: in each round, the ability used is read from the keyboard.

Knowing the description of each ability, the basic attributes for each character, which is the player character and what abilities 
they know, as well as each fight they go through, display for each fight the characters' attributes at the end of each round. 
Separate fights with an empty line.

At the beginning, the player character has 0 experience points, regardless of their level.

No ability that causes damage (DMG) can cause 0 damage; the minimum value is 1.

Attribute-modifying effects will not have stage values greater than 20.

All damage-causing effects (DMG) will have power (PWR) of at least 1. All characters will have all basic attributes at least equal to 1.

Input:

On the first line, the name of the file containing the description of all abilities.

On the second line, the name of the file containing the description of all characters.

On the third line, separated by spaces, the name of the player character, their level, control mode (EASY / MEDIUM / ADVANCED / CUSTOM), 
and the names of all abilities they know.

Until the end, information about fights, as follows:

On the first line, separated by spaces, the name of the enemy character, their level, control mode (EASY / MEDIUM / ADVANCED / CUSTOM), 
and the names of all abilities they know.

On the next line, if applicable, separated by spaces, the abilities used by the character with the CUSTOM control mode.

Data from the file:

The file containing the description of all abilities is structured as follows:

An unknown number of lines, each line containing the description of an ability. The information is separated by spaces as follows:

The name of the ability and an unknown number of effects until the end of the line. The effects can be:

● DMG PWR TYPE_ATK

● SELF_MOD ATR STAGE

● ENEMY_MOD ATR STAGE

The file containing the description of all characters is structured as follows:

An unknown number of lines, each line containing the description of a character. The information is separated by spaces as follows:

NAME BASE_HP BASE_ATK BASE_DEF BASE_SP_ATK BASE_SP_DEF BASE_SPD

Output:

For each fight, the characters' attributes at the beginning of the fight as well as at the end of each round. Always display information about the 
player character on the first line, regardless of who was first in that round.

HP ATK DEF SP_ATK SP_DEF SPD

The attributes displayed are the effective attributes (Life modified by damage, other attributes modified by specific abilities). 

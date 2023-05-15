# Meta-Math

## Elijah Delavar

### Compilation

TODO: Compilation Instructions (save for later)

### Description

Just a normal math quiz.  Nothing more...

TODO: Game Description (save for later)

- Developer Overview (Delete Later)

This game starts as a normal math quiz wherein an AI assistant poses each question
    and generally guides the player through the quiz.
The AI has a snarky/sarcastic personality and tends to insult the player.
However, the AI should have some sort of redeeming quality so that it is 
    not totally detestable.
That redeeming quality is yet to be developed.
The math quiz should act like a Choose Your Own Adventure Story with multiple
    splitting paths based on user input, and it should have different endings
    depending on which paths were used.
The game should have functionality which handles the player changing the Game Files.

#### Endings

TODO: Make secret some of the endings.

NOTE: Developer View!

- Competence    (CMP)

Player correctly answered all questions first time.

- Incompetence  (INC)

Player answered at least one question incorrect at some point in time.

- Retard        (RET)

Player answered all MDM questions incorrectly.

- Error         (ERR)

Player changed the game files or somehow made an illegal game state.

### TODOS

Each TODO item should be formatted like so:
- {TODO}: {Contributer's Name}

- Create checkState Method: Elijah

Check the state of the game to see which path to go on next.
Incorporate options from CSV file, if possible.

Status: Completed.

- Add Competence Ending:

This ending is achieved when the player correctly answers
    all of the questions the first time around.

Status: Unassigned.

- Add Incompetence Ending:

This ending is achieved when the player answers all of the
    questions, yet either had to redo some of them or
    flat-out got some of them wrong.

Status: Unassigned.

- Add Mentally-Disabled Mode (MDM):

When the player gets the first three questions wrong,
    the AI will turn on Mentally Disabled Mode,
    voicing the questions with caveman speech.
The AI will also ask the player if they want to turn
    the MDM if the player incorrectly answers the first
    two questions.
The player can exit MDM only by answering all of the
    questions correctly (or by manipulating the Game
    Files... somehow).

Status: Unassigned.

- Add Retard Ending:

This ending is achieved when the player incorrectly answers
    all of the MDM questions.

Status: Unassigned.

- Add Auto Save:

Automatically save the game every time the player answers a
    question.

Status: Unassigned.

- Add Path Memory: Elijah

Game remembers the player's choices.
Helps with deciding which ending to go to.

Status: Pending.

- Force Lowercase Answers: Elijah

Check the lowercase version of the player's answers using tolower()

Status: Pending.

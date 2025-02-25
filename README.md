# Game Design Document

## Section 0: Summary
The game, Running Dog, is an endless runner game where an avatar, a dog, must dodge obstacles in order to accumulate as many points as they can. The game ends when the avatar runs out of lives. There will be power ups throughout the game that the player can attempt to retrieve that will make it easier for the player to earn points. The members of the team creating this game are Ryan Hu, Allison Xin, and Clara Yu. 

## Section 1: Gameplay
The game progresses continuously as the player navigates through a challenging array of obstacles by jumping or ducking. It does not end until the moment the avatar, controlled by the player, runs out of lives (by hitting obstacles), resulting in a game over. This will mark the end of the current session, allowing the player to try again and play another round to improve their performance. Because the game is an endless runner platform, there is no win condition. Rather, the goal of the game is to deter loss for as long as possible and to accumulate the greatest number of points as possible. However, in the end, a player will always eventually lose. This occurs when the avatar hits too many obstacles and runs out of lives, causing the game to be over. In addition, there are no levels. Instead, after the conclusion of each round, the player can choose to try again in an attempt to earn more points. Points will represent the player's progress as they increase gradually over time at a rate of 100 point per second. This continuous accumulation adds a sense of progress and accomplishment, even during moments of inactivity. Points will serve as a measure of achievement, enriching the player's experience and encouraging replayability.
    
In the game, the incorporation of a physics engine is necessary, particularly focusing on gravity and collision dynamics. The engine will simulate basic interactions like gravity, collisions, and velocity, ensuring that the movement of the player follows realistic physical principles. Gravity will exert a constant downward force on the dinosaur, influencing its jumping mechanics, with the player initiating jumps to counteract gravity's pull. Collision detection will be integrated to manage interactions with obstacles, rendering power ups or lives loss.

The game starts with the player controlling an avatar in a barren landscape. As the game commences, the avatar begins running automatically, and the player's primary objective is to navigate it through various obstacles. Points accumulate gradually over time, incentivizing the player to survive for as long as possible. The player can make the avatar jump by pressing the up and down keys, enabling it to avoid obstacles. Along the way, power-ups like invincibility or additional lives appear, providing temporary advantages. Ultimately, the game ends when the avatar collides with an obstacle to the point of losing all 3 lives, at which point the player's leaderboard with the top 3 scores are displayed alongside options to restart the game. With each playthrough, players strive to surpass their previous high score, ensuring endless replayability and an immersive gaming experience.

In our game, we'll utilize a combination of sprites and vector graphics to create a visually appealing and dynamic gaming experience. The avatar, obstacles, and power-ups will be represented using sprite-based graphics, allowing for detailed animations and expressive movements that enhance immersion. Background elements such as the landscape and environmental effects will be crafted using vector graphics, providing scalability and crisp visuals across different screen sizes and resolutions. This approach ensures that the game maintains a cohesive aesthetic while allowing for flexibility in design and optimization for performance. 


## Section 2: Feature Set
Character/body sprites will be implemented as a priority 1 feature and will include drawn implementations of the bodies of the avatar and obstacles, and their respective graphics. Key handler and text rendering will be implemented as a priority 1 feature. The up and down keys will be used for jumping over and ducking under obstacles. Text Rendering will be implemented as well as for the “Game Over” and “Start Game” at the beginning and end of the game. Side scrolling background will be implemented as a priority 1 feature. This will cover the moving foreground.

UI elements will be implemented as a priority 2 feature. This will primarily include the button that the user presses to start the game, pause/resume buttons during the game, and rendering the current score during the game. Forces and ground collisions will be implemented as a priority 2 feature, which includes the gravity in the jumping motion of the avatar when the user presses the up key and the collision of the avatar with the ground. Obstacle Collisions will be implemented as a priority 2 feature. This involves checking if the bounding boxes of the avatar and an obstacle overlap. If the avatar collides with an obstacle, then a life is lost.

Power-ups will be implemented as a priority 3 feature. We will design different power-ups (e.g. invincibility, extra lives) and power downs (e.g. loss of points) and define their effects on the avatar. For example, a slow-mo might decrease the background speed, while invincibility could temporarily protect it from collisions. Lives will be implemented as a priority 3 feature. The user will have three lives. Hitting an obstacle will cause a life to be lost until all three lives are used up and the game ends. Game Animations will be implemented as a priority 3 feature. This will involve multiple starting and ending animations, as well as small in-game animations that signify different conditions in the game. For example, our avatar might jump onto the screen when the game begins, or start flashing when a life is lost.

Help Page will be implemented as a priority 4 feature. This will consist of a button on the starting screen that leads to a separate page that explains the rules of the game. This page will also explain how to play the game and will define each of the obstacles and power ups/downs. This will help the user to know how to play the game and how the game works as a whole before starting. Parallax background will be implemented as priority 4 feature where a background with mountains moves slower than the foreground. Leaderboard (high score) will be implemented as a priority 4 feature. This will be at the end of the game and show the current top 3 high scores and update if the newest run of the game resulted in a high score.

## Section 3: Timeline

| Priority | Feature | Group Member | Week |
| ----- | ----- | ----- | ----- | 
| Priority 1 | Character/Body Sprites | Clara | 1 |
| Priority 1 | Key Handler & Text Rendering | Allison | 1 | 
| Priority 1 | Side Scrolling Background | Ryan | 1 |
| Priority 2 | Obstacle Collisions | Clara | 2 |
| Priority 2 | UI Elements | Allison | 2 | 
| Priority 2 | Forces & Ground Collisions | Ryan | 2 |
| Priority 3 | Power-ups | Clara | 3 |
| Priority 3 | Lives | Allison | 3 | 
| Priority 3 | Game Animations | Ryan | 3 |
| Priority 4 | Help Page | Clara | 3 |
| Priority 4 | Leaderboard(High Score) | Allison | 3 | 
| Priority 4 | Parallax Background | Ryan | 3 |
## Section 4: Disaster Recovery
Our entire group agreed that this is how we will get back on track if either of us falls behind. If one of our group members is falling behind, they will promptly reach out to the rest of the group for potential insight and assistance with debugging. Ideally, the combined knowledge of the group will be able to fix any issues at hand. However, if the problem seems to be outside our capabilities, the group will collectively attend office hours together to fix the problem with help of the TAs.
In the off chance that the TAs are also unable to help figure out our problem, we will be open and willing to potentially reconsider and modify their planned features. The core functionality of our game will remain the same, but by reworking the problematic elements, the group can continue making progress. 

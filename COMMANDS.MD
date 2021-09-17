# Monster M4sk Puppet Control

The code in `user_voodoo_json.cpp` allows the m4sk "puppet" code to accept commands as a JSON payload via the board's serial connection to a controlling device running a python script.

The basic format of the JSON payload looks like this:

`{"command": "$COMMAND"}`

## Commands
Valid commands are `speak`, `loop` and `animate`. For each, there are additional key/value pairs that can be included in the payload.

|Command|Description|Additional Settings|Example|
|---|---|---|---|
|`speak`|Triggers the mask's "speaking" LED animation.|`speakTime`: Sets the length of time that the mask "sparkles" while speaking, in milliseconds.|`{"command": "speak", "speakTime": 3500}`|
|`loop`|Triggers the mask's "look around randomly" loop.|`transitionTime`: Sets the length of time the mask waits while looking around before it will accept new commands. Allows animations to complete. Set in ms.|`{"command" "loop", "transitionTime": 2000}`|
|`animate`|Triggers a specific animation behavior in the mask.|`transitionTime`, used as above. Additionally, the name of the animation to trigger as `animation`. The list of animations is in the table below this one.|`{"command": "animate", "animation": "look_other", "transitionTime": 3000}`|

### Animations
|Animation|Description|
|---|---|
|`look_mme`|Causes the mask to look the direction of the the crystal ball.|
|`look_other`|Causes the mask to look in the direction of the other mask on the wall.|
|`look_front`|Causes the mask to look at the trick-or-treaters.|
|`look_left`|Causes the mask to look to its left.|
|`look_right`|Causes the mask to look to its right.|
|`look_up`|Causes the mask to look up.|
|`look_down`|Causes the mask to look down.|

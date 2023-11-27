bool holdtoPress(bool reset)
{
	static int DMkeyHoldCounter = 0; //DM - DevMode
	if(reset)
		DMkeyHoldCounter = 0;
	const int DMkeyHoldThreshold = 30; 
	if (DMkeyHoldCounter <= 0) {
			DMkeyHoldCounter = DMkeyHoldThreshold;  // Reset the counter when state changes
			return true;
	} else {
			DMkeyHoldCounter--;  // Decrease the counter if key is held
			return false;
	}
	return false;
}

bool handleKeyAction(bool keyCondition, int &counter, int threshold) 
{
    if (keyCondition) {
        if (counter <= 0) {
            counter = threshold;  // Reset the counter
            return true;  // Indicate that the action should be performed
        } else {
            counter--;  // Decrease the counter if key is held
        }
    } else {
        counter = 0;  // Reset the counter if key is not pressed
    }
    return false;  // Indicate that the action should not be performed
}
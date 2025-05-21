module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationEnums;

export namespace PGUI::UI::Animation
{
	enum class AnimationManagerStatus
	{
		Idle = UI_ANIMATION_MANAGER_IDLE,
		Busy = UI_ANIMATION_MANAGER_BUSY
	};

	enum class AnimationMode
	{
		Disabled = UI_ANIMATION_MODE_DISABLED,
		SystemDefault = UI_ANIMATION_MODE_SYSTEM_DEFAULT,
		Enabled = UI_ANIMATION_MODE_ENABLED
	};

	enum class AnimationUpdateResult
	{
		NoChange = UI_ANIMATION_UPDATE_NO_CHANGE,
		VariablesChanged = UI_ANIMATION_UPDATE_VARIABLES_CHANGED
	};

	enum class AnimationSlope
	{
		Increasing = UI_ANIMATION_SLOPE_INCREASING,
		Decreasing = UI_ANIMATION_SLOPE_DECREASING
	};

	enum class AnimationRoundingMode
	{
		Nearest = UI_ANIMATION_ROUNDING_NEAREST,
		Floor = UI_ANIMATION_ROUNDING_FLOOR,
		Ceiling = UI_ANIMATION_ROUNDING_CEILING
	};

	enum class StoryboardStatus
	{
		Building = UI_ANIMATION_STORYBOARD_BUILDING,
		Scheduled = UI_ANIMATION_STORYBOARD_SCHEDULED,
		Cancelled = UI_ANIMATION_STORYBOARD_CANCELLED,
		Playing = UI_ANIMATION_STORYBOARD_PLAYING,
		Truncated = UI_ANIMATION_STORYBOARD_TRUNCATED,
		Finished = UI_ANIMATION_STORYBOARD_FINISHED,
		Ready = UI_ANIMATION_STORYBOARD_READY,
		InsufficentPriority = UI_ANIMATION_STORYBOARD_INSUFFICIENT_PRIORITY
	};

	enum class AnimationSchedulingResult
	{
		UnexpectedFailure = UI_ANIMATION_SCHEDULING_UNEXPECTED_FAILURE,
		InsufficientPriority = UI_ANIMATION_SCHEDULING_INSUFFICIENT_PRIORITY,
		AlreadyScheduled = UI_ANIMATION_SCHEDULING_ALREADY_SCHEDULED,
		Succeeded = UI_ANIMATION_SCHEDULING_SUCCEEDED,
		Deferred = UI_ANIMATION_SCHEDULING_DEFERRED
	};
}
module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationVariableChangeEventHandler;

import std;

import PGUI.Event;
import :AnimationInterface;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	class AnimationVariableChangeEventHandlerRouter final : public IUIAnimationVariableChangeHandler2
	{
		using AnimationVariableChangeHandler = std::function<HRESULT(
			Storyboard, AnimationVariable,
			std::span<double>, std::span<double>)>;

		public:
		AnimationVariableChangeEventHandlerRouter() noexcept = default;

		explicit AnimationVariableChangeEventHandlerRouter(const AnimationVariableChangeHandler& handler) noexcept;

		auto __stdcall QueryInterface(const IID& iid, void** obj)->HRESULT override;

		auto __stdcall AddRef()->ULONG override;

		auto __stdcall Release()->ULONG override;

		auto __stdcall OnValueChanged(
			IUIAnimationStoryboard2* storyboard,
			IUIAnimationVariable2* variable,
			DOUBLE* newValues, DOUBLE* previousValues,
			UINT dimension)->HRESULT override;

		auto SetHandler(const AnimationVariableChangeHandler& handler) noexcept -> void;

		auto ClearHandler() noexcept -> void;

		private:
		std::mutex handlerMutex;
		AnimationVariableChangeHandler handler;
		LONG referenceCount = 1;
	};

	class AnimationVariableIntegerChangeEventHandlerRouter final : public IUIAnimationVariableIntegerChangeHandler2
	{
		using AnimationIntegerVariableChangeHandler = std::function<HRESULT(
			Storyboard, AnimationVariable,
			std::span<INT32>, std::span<INT32>)>;

		public:
		AnimationVariableIntegerChangeEventHandlerRouter() noexcept = default;

		explicit AnimationVariableIntegerChangeEventHandlerRouter(const AnimationIntegerVariableChangeHandler& handler) noexcept;

		auto __stdcall QueryInterface(const IID& iid, void** obj)->HRESULT override;

		auto __stdcall AddRef()->ULONG override;

		auto __stdcall Release()->ULONG override;

		auto __stdcall OnIntegerValueChanged(
			IUIAnimationStoryboard2* storyboard,
			IUIAnimationVariable2* variable,
			INT32* newValues, INT32* previousValues,
			UINT dimension)->HRESULT override;

		auto SetHandler(const AnimationIntegerVariableChangeHandler& handler) noexcept -> void;

		auto ClearHandler() noexcept -> void;

		private:
		std::mutex handlerMutex;
		AnimationIntegerVariableChangeHandler handler;
		LONG referenceCount = 1;
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationVariableChangeEventHandler
	{
		public:
		AnimationVariableChangeEventHandler() noexcept;

		virtual ~AnimationVariableChangeEventHandler() = default;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }
		[[nodiscard]] auto& GetIntegerRouter() noexcept { return integerRouter; }
		[[nodiscard]] const auto& GetIntegerRouter() const noexcept { return integerRouter; }

		virtual auto OnVariableChanged(
			Storyboard storyboard, AnimationVariable variable,
			std::span<double> newValues, std::span<double> previousValues) -> void = 0;

		virtual auto OnVariableIntegerChanged(
			Storyboard storyboard, AnimationVariable variable,
			std::span<INT32> newValues, std::span<INT32> previousValues) -> void = 0;

		private:
		AnimationVariableChangeEventHandlerRouter router;
		AnimationVariableIntegerChangeEventHandlerRouter integerRouter;

		auto CallVariableChanged(
			const Storyboard& storyboard, const AnimationVariable& variable,
			std::span<double> newValues, std::span<double> previousValues) noexcept -> HRESULT;

		auto CallVariableIntegerChanged(
			const Storyboard& storyboard, const AnimationVariable& variable,
			std::span<INT32> newValues,
			std::span<INT32> previousValues) noexcept -> HRESULT;
	};

	class AnimationVariableChangeEvent final : public AnimationVariableChangeEventHandler
	{
		public:
		[[nodiscard]] auto& VariableChanged() noexcept { return variableChangedEvent; }
		[[nodiscard]] const auto& VariableChanged() const noexcept { return variableChangedEvent; }
		[[nodiscard]] auto& VariableIntegerChanged() noexcept { return variableIntegerChangedEvent; }
		[[nodiscard]] const auto& VariableIntegerChanged() const noexcept { return variableIntegerChangedEvent; }

		private:
		Event<Storyboard, AnimationVariable,
			std::span<double>, std::span<double>> variableChangedEvent;
		Event<Storyboard, AnimationVariable,
			std::span<INT32>, std::span<INT32>> variableIntegerChangedEvent;

		auto OnVariableChanged(
			Storyboard storyboard, AnimationVariable variable,
			std::span<double> newValues, std::span<double> previousValues) -> void override;

		auto OnVariableIntegerChanged(
			Storyboard storyboard, AnimationVariable variable,
			std::span<INT32> newValues, std::span<INT32> previousValues) -> void override;
	};
}

#pragma once
#include "AbstractPin.hpp"

class OutputPin;

class InputPin : public AbstractPin
{
protected:
	OutputPin* connection = nullptr;
	ax::NodeEditor::LinkId linkID = -1;

public:
	InputPin() = default;
	InputPin(AbstractPort* port);
	InputPin(InputPin&&) = default;
	InputPin(InputPin const&) = delete;
	InputPin& operator=(InputPin&&) = default;
	InputPin& operator=(InputPin const&) = delete;
	virtual ~InputPin() = default;

protected:
	ImVec2 calculateAnchorPosition() const override;

public:
	virtual void drawLink() = 0;
	bool canConnect(AbstractPin* outputPin) final override;
	void connect(AbstractPin* outputPin) final override;

};
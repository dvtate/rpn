#ifndef YODA_TOP_MODIFIERS_HPP
#define YODA_TOP_MODIFIERS_HPP


// recursively evaluate the top value
CalcValue* conv_top(std::stack<CalcValue>& mainStack, std::vector<UserVar>& var_nodes, bool showErrors, std::vector<void*>& freeable){
	CalcValue* p = NULL, *ret = NULL;

	if (mainStack.top().type == CalcValue::REF) {
		ret = mainStack.top().valAtRef(var_nodes);
		mainStack.pop();
		return ret;
	}
	if (mainStack.top().type == CalcValue::INX) {
		std::vector <ssize_t> inx;
		GET_LIST_INDEX(mainStack, var_nodes, inx);
		CalcValue* cv = conv_top(mainStack, var_nodes, showErrors, freeable);
		if (!cv)
			printf("fml\n");
		if (!cv || cv->type != CalcValue::ARR) {
			PASS_ERROR("\aERROR: index without list\n");
		} else {
			ret = cv->getListElem(inx);
			if (!cv) {
				PASS_ERROR("\aERROR: list index out of bounds\n");
			}
			return ret;
		}
	}
	if (mainStack.top().type == CalcValue::REQ) {
		// <literal> :scope :scope
		if (mainStack.top().request->at(0) == " ") {
			std::vector<std::string> request = *mainStack.top().request;
			mainStack.pop();
			CalcValue* cv = conv_top(mainStack, var_nodes, showErrors, freeable);
			if (cv->type != CalcValue::OBJ) {
				PASS_ERROR("\aERROR: member request without object\n");
			}
			ret = cv->requestMember(request);
			if (!ret) {
				PASS_ERROR("\aERROR: invalid member request");
			}
			return ret;

			// $variable :scope :scope
		} else {
			ret = mainStack.top().requestMember(var_nodes);
			if (!ret) {
				PASS_ERROR("\aERROR: object member not found\n");
			}
			mainStack.pop();
			return ret;
		}
	}
	ret = new CalcValue(mainStack.top());
	freeable.push_back(ret);
	mainStack.pop();
	return ret;

}

CalcValue* get_top(std::stack<CalcValue>& mainStack, std::vector<UserVar>& var_nodes, bool showErrors, std::vector<void*>& freeable){
	CalcValue* p = NULL, *ret = NULL;

	if (mainStack.top().type == CalcValue::REF) {
		ret = mainStack.top().valAtRef(var_nodes);
		// if the variable doesn't exist, assign it to null
		if (!ret)
			ret = &vars::assignNewVar(var_nodes, mainStack.top().string, CalcValue())->val;

		mainStack.pop();
		return ret;
	}
	if (mainStack.top().type == CalcValue::INX) {
		std::vector <ssize_t> inx;
		GET_LIST_INDEX(mainStack, var_nodes, inx);
		CalcValue* cv = conv_top(mainStack, var_nodes, showErrors, freeable);
		if (!cv || cv->type != CalcValue::ARR) {
			PASS_ERROR("\aERROR: index without list\n");
		}

		ret = cv->getListElem(inx);
		if (!cv) {
			PASS_ERROR("\aERROR: list index out of bounds\n");
		}
		return ret;
	}
	if (mainStack.top().type == CalcValue::REQ) {
		// <literal> :scope :scope
		if (mainStack.top().request->at(0) == " ") {
			std::vector<std::string> request = *mainStack.top().request;
			mainStack.pop();
			CalcValue* cv = conv_top(mainStack, var_nodes, showErrors, freeable);
			if (cv->type != CalcValue::OBJ) {
				PASS_ERROR("\aERROR: member request without object\n");
			}
			ret = cv->requestMember(request);
			if (!ret) {
				PASS_ERROR("\aERROR: invalid member request");
			}
			return ret;

		}
		// $variable :scope :scope
		// TODO: make anything that isnt there already...
		ret = mainStack.top().requestMember(var_nodes);
		if (!ret) {
			PASS_ERROR("\aERROR: object member not found\n");
		}
		mainStack.pop();
		return ret;
	}
	ret = new CalcValue(mainStack.top());
	freeable.push_back(ret);
	mainStack.pop();
	return ret;

}

#endif //YODA_TOP_MODIFIERS_HPP
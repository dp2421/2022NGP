#pragma once

class Interaction_Object {
public:
	int x;
	int y;
	bool state = false;
	INTER_OBJ type;

public:
	void SetCollsionBox();
	void Update();
	void Collsion();

	CImage& getDrawImageByState()
	{
		switch (type)
		{
		case INTER_OBJ::REVER:
			if (state)
				return Manager::GetInstance().rever_pull;
			else
				return Manager::GetInstance().rever_nomal;
		case INTER_OBJ::DOOR:
			return Manager::GetInstance().door;
		case INTER_OBJ::BUTTON:
			if (state)
				return Manager::GetInstance().button_pull;
			else
				return Manager::GetInstance().button_normal;
		case INTER_OBJ::PORTAL:
			return Manager::GetInstance().portal_img;
		default:
			break;
		}
	}
};

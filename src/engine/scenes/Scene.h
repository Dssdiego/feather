//
// Created by Diego S. Seabra on 17/12/22.
//

#ifndef FEATHER_SCENE_H
#define FEATHER_SCENE_H

class Scene
{
public:
	//Scene();
	//~Scene();

	virtual void Init()     {};
	virtual void Update()   {};
	virtual void Draw()     {};
	virtual void Shutdown() {};

private:
	uint32_t mId;
	std::string mName;
	//std::vector<>
}

#endif //FEATHER_SCENE_H

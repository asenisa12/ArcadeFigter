#include "GameObjectFactory.h"

bool GameObjectFactory::registerType(std::string typeID, BaseCreator* pCreator)
{
	std::map<std::string, BaseCreator*>::iterator it =
		objectCreators.find(typeID);

	if (it == objectCreators.end())
	{
		delete pCreator;
		return false;
	}

	objectCreators[typeID] = pCreator;
	return true;
}

GameObject* GameObjectFactory::create(std::string typeID)
{
	std::map<std::string, BaseCreator*>::iterator it =
		objectCreators.find(typeID);

	if (it == objectCreators.end())
	{
		printf("could not find type: %s\n", typeID.c_str());
		return NULL;
	}
	BaseCreator* pCreator = (*it).second;
	return pCreator->createGameObject();
}


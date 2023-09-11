#include "EngineSetting.h"

using namespace std;

namespace BJEngine
{
	const int EngineSetting::LAYER_MAX_SIZE = 20;
	const bool EngineSetting::SHOWDEBUGINFO_DEFAULT = true;
	const bool EngineSetting::DEBUGTOGGLEKEYALLOWED_DEFAULT = true;

	int EngineSetting::GetLayerID(wstring layername)
	{
		if (layername == L"")
		{
			return -1;
		}

		for (int i = 0; i < layer.size(); i++)
		{
			if (layer[i] == layername)
			{
				return i;
			}
		}

		return -1;
	}

	void EngineSetting::SetLayerName(int layerid, wstring layername)
	{
		if (0 <= layerid && layerid < layer.size())
		{
			layer[layerid] = layername;
		}
	}

	wstring EngineSetting::GetLayerName(int layerid)
	{
		if (0 <= layerid && layerid < layer.size())
		{
			return layer[layerid];
		}

		return L"";
	}

	void EngineSetting::SetCollisionSetting(bool collision, int layerid1, int layerid2)
	{
		if (layerid1 < 0 || collisionSetting.size() <= layerid1 || layerid2 < 0 || collisionSetting.size() <= layerid2)
		{
			return;
		}

		if (layer[layerid1] == L"")
		{
			return;
		}

		if (layer[layerid2] == L"")
		{
			return;
		}

		collisionSetting[layerid1][layerid2] = collision;
		collisionSetting[layerid2][layerid1] = collision;
	}

	bool EngineSetting::GetCollisionSetting(int layerid1, int layerid2)
	{
		if (layerid1 < 0 || collisionSetting.size() <= layerid1 || layerid2 < 0 || collisionSetting.size() <= layerid2)
		{
			return false;
		}

		if (layer[layerid1] == L"")
		{
			return false;
		}

		if (layer[layerid2] == L"")
		{
			return false;
		}

		return collisionSetting[layerid1][layerid2];
	}

	void EngineSetting::SetDebugToggleKeyAllowed(bool value)
	{
		debugToggleKeyAllowed = value;
	}

	bool EngineSetting::GetDebugToggleKeyAllowed()
	{
		return debugToggleKeyAllowed;
	}

	void EngineSetting::SetShowDebugInfo(bool value)
	{
		showDebugInfo = value;
	}

	bool EngineSetting::GetShowDebugInfo()
	{
		return showDebugInfo;
	}

	EngineSetting& EngineSetting::GetInstance()
	{
		static EngineSetting instance;
		return instance;
	}

	EngineSetting::EngineSetting() :
		engineMode(EngineSetting::Mode::MODE_2D),
		graphicsEngine(EngineSetting::GraphicsEngine::DIRECT2D_GRAPHICS),
		showDebugInfo(SHOWDEBUGINFO_DEFAULT),
		debugToggleKeyAllowed(DEBUGTOGGLEKEYALLOWED_DEFAULT),
		layer(vector<wstring>(LAYER_MAX_SIZE, L"")),
		collisionSetting(vector<vector<bool>>(LAYER_MAX_SIZE, vector<bool>(LAYER_MAX_SIZE, false)))
	{
		layer[0] = L"Default";
	}

	EngineSetting::~EngineSetting()
	{

	}
}
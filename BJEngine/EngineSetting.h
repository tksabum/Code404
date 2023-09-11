#pragma once

#ifdef ENGINESETTING_EXPORTS
#define ENGINESETTING_API __declspec(dllexport)
#else
#define ENGINESETTING_API __declspec(dllimport)
#endif

#include <vector>
#include <string>

namespace BJEngine
{
	class EngineSetting
	{
	public:
		enum class Mode
		{
			MODE_2D,
			MODE_3D
		};

		enum class GraphicsEngine
		{
			DIRECT2D_GRAPHICS,
			DIRECT2D_DC
		};

		Mode engineMode;
		GraphicsEngine graphicsEngine;

		// LayerID를 얻어온다. 실패시 -1을 리턴한다.
		ENGINESETTING_API int GetLayerID(std::wstring layername);

		ENGINESETTING_API void SetLayerName(int layerid, std::wstring layername);
		ENGINESETTING_API std::wstring GetLayerName(int layerid);

		ENGINESETTING_API void SetCollisionSetting(bool collision, int layerid1, int layerid2);
		ENGINESETTING_API bool GetCollisionSetting(int layerid1, int layerid2);

		ENGINESETTING_API void SetDebugToggleKeyAllowed(bool value);
		ENGINESETTING_API bool GetDebugToggleKeyAllowed();

		ENGINESETTING_API void SetShowDebugInfo(bool value);
		ENGINESETTING_API bool GetShowDebugInfo();
		
		const static int LAYER_MAX_SIZE;
		const static bool SHOWDEBUGINFO_DEFAULT;
		const static bool DEBUGTOGGLEKEYALLOWED_DEFAULT;
	
	private:
		bool showDebugInfo;
		bool debugToggleKeyAllowed;
		std::vector<std::wstring> layer;
		std::vector<std::vector<bool>> collisionSetting;


		//singleton
	public:
		ENGINESETTING_API static EngineSetting& GetInstance();

	private:
		ENGINESETTING_API EngineSetting();
		ENGINESETTING_API ~EngineSetting();
		ENGINESETTING_API EngineSetting(const EngineSetting& ref);
		ENGINESETTING_API EngineSetting& operator=(const EngineSetting& ref) {}
	};
}
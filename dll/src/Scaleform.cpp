#include <skse.h>
#include <skse/PluginAPI.h>
#include <skse/ScaleformCallbacks.h>
#include <skse/ScaleformExtendedData.h>
#include <skse/ScaleformMovie.h>
#include <skse/GlobalLocks.h>

#include "Scaleform.h"
#include "Hooks.h"



class SKSEScaleform_RequestTargetInfo : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args* args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);
		ASSERT(args->args[0].GetType() == GFxValue::kType_Array);

		GFxValue* arr = &args->args[0];
		RegisterNumber(arr, "formID", g_targetFormID);
		RegisterNumber(arr, "posX", g_targetPosX);
		RegisterNumber(arr, "posY", g_targetPosY);
		RegisterNumber(arr, "dist", g_targetDist);
		GFxValue	fxValue;
		fxValue.SetString(g_targetName);
		arr->SetMember("targetName", &fxValue);

		g_loadGameLock.Leave();
	}
};


namespace Scaleform
{
	bool RegisterCallback(GFxMovieView * view, GFxValue * root)
	{
		RegisterFunction <SKSEScaleform_RequestTargetInfo>(root, view, "RequestTargetInfo");
		return true;
	}
}

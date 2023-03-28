#include "solpch.h"
#include "Sol/Utils/PlatformUtils.h"
#include "Sol/Core/Application.h"

#include <filesystem>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Sol {
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
	
		std::string assetsFolder = GetAssetsFolder();
		ofn.lpstrInitialDir = assetsFolder.c_str();

		GetAssetsFolder();

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{

			std::string selectedFolder = ofn.lpstrFile;

			// Ensure that the selected folder is within the "assets" folder or a subdirectory of it	
			if (selectedFolder.compare(0, assetsFolder.length(), assetsFolder) == 0)
			{
				return selectedFolder;
			}
			else 
			{
				MessageBoxA(NULL, "Cannot open file. File must be located in the asset folder or its subdirectories.", "Error", MB_ICONERROR);
				return std::string();
			}
		}

		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		std::string assetsFolder = GetAssetsFolder();
		ofn.lpstrInitialDir = GetAssetsFolder().c_str();

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			std::string selectedFolder = ofn.lpstrFile;

			std::string assetsFolder = GetAssetsFolder();
			if (selectedFolder.compare(0, assetsFolder.length(), assetsFolder) == 0)
			{
				return selectedFolder;
			}
			else
			{
				MessageBoxA(NULL, "Cannot save file. File must be stored in the asset folder or its subdirectories.", "Error", MB_ICONERROR);
				return std::string();
			}
		}

		return std::string();
	}

	std::string FileDialogs::GetAssetsFolder()
	{
		std::string rootDir = std::filesystem::current_path().string();
		std::string assetPath = rootDir + "\\assets";
		return assetPath;
	}
}
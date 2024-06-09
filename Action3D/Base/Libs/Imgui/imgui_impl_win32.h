// -----------------------------------------------------------------------------------------------------------  // -- 2020.8.7
// 
// ImGui Windows API �p�̃w�b�_�t�@�C��
// 
//   ���{����͂ɑΉ����邽�߂Ɖ�ʂ̊g�k�E�t���X�N���[���ɑΉ����邽�߂ɃJ�X�^�}�C�Y���Ă���
//   �J�X�^�}�C�Y�ӏ��ɂ́A�u// -- 2020.8.7�v�̈󂪂��Ă���B
//   �܂��A�ύX�O�̉ӏ��ɂ́A�u// ���̓��e�v�̈󂪂��Ă���B
//   
// -----------------------------------------------------------------------------------------------------------

// dear imgui: Platform Binding for Windows (standard windows API for 32 and 64 bits applications)
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

// Implemented features:
//  [X] Platform: Clipboard support (for Win32 this is actually part of core imgui)
//  [X] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [X] Platform: Keyboard arrays indexed using VK_* Virtual Key Codes, e.g. ImGui::IsKeyPressed(VK_SPACE).
//  [X] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.

#pragma once

// ��ʂ̊g�k�����̒ǉ��ɍ��킹�ĉ�ʂ̏����T�C�Y�𓾂�悤�Ɋ֐����C��       // -- 2020.8.7
IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd, int WidthIn, int HeightIn);    // -- 2020.8.7
//IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd);       // ���̓��e

IMGUI_IMPL_API void     ImGui_ImplWin32_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplWin32_NewFrame();

// Handler for Win32 messages, update mouse/keyboard data.
// You may or not need this for your implementation, but it can serve as reference for handling inputs.
// Intentionally commented out to avoid dragging dependencies on <windows.h> types. You can COPY this line into your .cpp code instead.
/*
IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
*/

#include "UIWidgets.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace Pinecone
{
	namespace UI
	{
		bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue, float columnWidth)
		{
			bool valueChanged = false;
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();

			ImGui::Spacing();

			return valueChanged;
		}

		bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
		{
			bool valueChanged = false;
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();

			ImGui::Spacing();

			return valueChanged;
		}

		bool DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
		{
			bool valueChanged = false;
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.9f, 0.85f, 0.35f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.95f, 0.45f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.9f, 0.85f, 0.35f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("W", buttonSize))
				values.w = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f") ? valueChanged = true : valueChanged = valueChanged;
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();

			ImGui::Spacing();

			return valueChanged;
		}

		// Taken from "imgui_widgets.cpp" but modified to have a texture between the arrow/bullet
		static bool TreeNodeBehaviourWithImage(ImTextureID user_texture_id, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
			const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

			if (!label_end)
				label_end = ImGui::FindRenderedTextEnd(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, label_end, false);

			// We vertically grow up to current line height up the typical widget height.
			const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
			ImRect frame_bb;
			frame_bb.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
			frame_bb.Min.y = window->DC.CursorPos.y;
			frame_bb.Max.x = window->WorkRect.Max.x;
			frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
			if (display_frame)
			{
				// Framed header expand a little outside the default padding, to the edge of InnerClipRect
				// (FIXME: May remove this at some point and make InnerClipRect align with WindowPadding.x instead of WindowPadding.x*0.5f)
				frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
				frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
			}

			const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);           // Collapser arrow width + Spacing
			const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
			const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
			ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
			ImGui::ItemSize(ImVec2(text_width, frame_height), padding.y);

			// For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
			ImRect interact_bb = frame_bb;
			if (!display_frame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
				interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;

			// Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
			// For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
			// This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
			const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
			bool is_open = ImGui::TreeNodeBehaviorIsOpen(id, flags);
			if (is_open && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);

			bool item_add = ImGui::ItemAdd(interact_bb, id);
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
			g.LastItemData.DisplayRect = frame_bb;

			if (!item_add)
			{
				if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
					ImGui::TreePushOverrideID(id);
				IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
				return is_open;
			}

			ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
			if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
				button_flags |= ImGuiButtonFlags_AllowItemOverlap;
			if (!is_leaf)
				button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;

			// We allow clicking on the arrow section with keyboard modifiers held, in order to easily
			// allow browsing a tree while preserving selection with code implementing multi-selection patterns.
			// When clicking on the rest of the tree node we always disallow keyboard modifiers.
			const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
			const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
			const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
			if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
				button_flags |= ImGuiButtonFlags_NoKeyModifiers;

			// Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
			// Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
			// - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
			// - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
			// - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
			// - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
			// - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
			// It is rather standard that arrow click react on Down rather than Up.
			// We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on the initial MouseDown in order for drag and drop to work.
			if (is_mouse_x_over_arrow)
				button_flags |= ImGuiButtonFlags_PressedOnClick;
			else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
				button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
			else
				button_flags |= ImGuiButtonFlags_PressedOnClickRelease;

			bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
			const bool was_selected = selected;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
			bool toggled = false;
			if (!is_leaf)
			{
				if (pressed && g.DragDropHoldJustPressedId != id)
				{
					if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
						toggled = true;
					if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
						toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
					if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
						toggled = true;
				}
				else if (pressed && g.DragDropHoldJustPressedId == id)
				{
					IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
					if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
						toggled = true;
				}

				if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open)
				{
					toggled = true;
					ImGui::NavMoveRequestCancel();
				}
				if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
				{
					toggled = true;
					ImGui::NavMoveRequestCancel();
				}

				if (toggled)
				{
					is_open = !is_open;
					window->DC.StateStorage->SetInt(id, is_open);
					g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
				}
			}
			if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
				ImGui::SetItemAllowOverlap();

			// In this branch, TreeNodeBehavior() cannot toggle the selection so this will never trigger.
			if (selected != was_selected) //-V547
				g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

			// Render
			const ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
			if (display_frame)
			{
				// Framed type
				const ImU32 bg_col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
				ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
				ImGui::RenderNavHighlight(frame_bb, id, nav_highlight_flags);
				if (flags & ImGuiTreeNodeFlags_Bullet)
					ImGui::RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);
				else if (!is_leaf)
					ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
				else // Leaf without bullet, left-adjusted text
					text_pos.x -= text_offset_x;
				if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
					frame_bb.Max.x -= g.FontSize + style.FramePadding.x;

				if (g.LogEnabled)
					ImGui::LogSetNextTextDecoration("###", "###");

				// Draw image
				const ImVec2& img_size = ImVec2(g.FontSize, g.FontSize);
				ImRect bb(text_pos, ImVec2{ text_pos.x + img_size.x, text_pos.y + img_size.y });

				window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, ImVec2(0, 1), ImVec2(1, 0));

				// Draw text
				ImGui::RenderTextClipped(ImVec2(text_pos.x + img_size.x + padding.x, text_pos.y), frame_bb.Max, label, label_end, &label_size);
			}
			else
			{
				// Unframed typed for tree nodes
				if (hovered || selected)
				{
					const ImU32 bg_col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
					ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
				}
				ImGui::RenderNavHighlight(frame_bb, id, nav_highlight_flags);
				if (flags & ImGuiTreeNodeFlags_Bullet)
					ImGui::RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), text_col);
				else if (!is_leaf)
					ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
				if (g.LogEnabled)
					ImGui::LogSetNextTextDecoration(">", NULL);

				// Draw image
				const ImVec2& img_size = ImVec2(g.FontSize, g.FontSize);
				ImRect bb(text_pos, ImVec2{ text_pos.x + img_size.x, text_pos.y + img_size.y });

				window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, ImVec2(0, 1), ImVec2(1, 0));

				// Draw text
				ImGui::RenderText(ImVec2(text_pos.x + img_size.x + padding.x, text_pos.y), label, label_end, false);
			}

			if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				ImGui::TreePushOverrideID(id);
			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
			return is_open;
		}

		// Taken from "imgui_widgets.cpp" but modified to have a texture between the arrow/bullet
		bool DrawTreeNodeWithImage(ImTextureID user_texture_id, const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			va_list args;
			va_start(args, fmt);
			const char* label, * label_end;
			ImFormatStringToTempBufferV(&label, &label_end, fmt, args);
			va_end(args);

			return TreeNodeBehaviourWithImage(user_texture_id, window->GetID(ptr_id), flags, label, label_end);
		}
	}
}

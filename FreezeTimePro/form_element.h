#pragma once

#include "common.h"

namespace FreezeTimePro {

	typedef int Color;

	enum class TextAlignment {
		TopLeft = DT_TOP | DT_LEFT,
		TopCenter = DT_TOP | DT_CENTER,
		TopRight = DT_TOP | DT_RIGHT,
		MiddleLeft = DT_VCENTER | DT_LEFT | DT_SINGLELINE,
		MiddleCenter = DT_VCENTER | DT_CENTER | DT_SINGLELINE,
		MiddleRight = DT_VCENTER | DT_RIGHT | DT_SINGLELINE,
		BottomLeft = DT_BOTTOM | DT_LEFT,
		BottomCenter = DT_BOTTOM | DT_CENTER,
		BottomRight = DT_BOTTOM | DT_RIGHT,
	};

	class FormFont {
	private:
		std::string FamilyName_;
		int Size_;
		bool Bold_;
		bool Italic_;
		bool Underline_;
		bool StrikeOut_;
		std::string Identity_;

	public:
		bool operator==(const FormFont &f) const {
			return Identity_ == f.Identity_;
		}

		HFONT Create(int logical_y) {
			return CreateFont(
				MulDiv(Size_, logical_y, 72), 0,
				0, 0, Bold_ ? FW_BOLD : FW_NORMAL, Italic_, Underline_, StrikeOut_, GB2312_CHARSET,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
				FamilyName_.c_str());
		}

	protected:
		inline void UpdateIdentity() {
			Identity_ = FamilyName_ + (Bold_ ? "__B" : "") + (Italic_ ? "__I" : "") +
				(Underline_ ? "__U" : "") + (StrikeOut_ ? "__S" : "") + "___" +
				Utility::ToString(Size_);
		}

	public:
		FormFont() : FamilyName_("Microsoft Yahei"), 
			Size_(12), Bold_(false), Italic_(false), 
			Underline_(false), StrikeOut_(false) {
			UpdateIdentity();
		}

		inline std::string GetIdentity() const {
			return Identity_;
		}
	};

	class FormGraphics {
	private:
		std::unordered_map<Color, HBRUSH> Brushes_;
		std::unordered_map<std::string, HFONT> Fonts_;

	private:
		HDC Handle_;
		int LogicalPixelX_;
		int LogicalPixelY_;
		std::string LastFontIdentity_;

	public:
		inline void Transform(RECT &r) {
			r.left = MulDiv(r.left, LogicalPixelX_, 72);
			r.right = MulDiv(r.right, LogicalPixelX_, 72);
			r.top = MulDiv(r.top, LogicalPixelY_, 72);
			r.bottom = MulDiv(r.bottom, LogicalPixelY_, 72);
		}

		inline void InverseTransform(RECT &r) {
			r.left = MulDiv(r.left, 72, LogicalPixelX_);
			r.right = MulDiv(r.right, 72, LogicalPixelX_);
			r.top = MulDiv(r.top, 72, LogicalPixelY_);
			r.bottom = MulDiv(r.bottom, 72, LogicalPixelY_);
		}

	public:
		FormGraphics() : Brushes_(), Fonts_(), LastFontIdentity_() {
		}

		~FormGraphics() {
			for (auto v : Brushes_) {
				DeleteObject(v.second);
			}
			for (auto v : Fonts_) {
				DeleteObject(v.second);
			}
		}

	public:
		HBRUSH GetBrush(Color color) {
			if (color < 0) {
				return NULL;
			}
			auto v = Brushes_.find(color);
			if (v == Brushes_.end()) {
				HBRUSH br = CreateSolidBrush(color);
				Brushes_.insert_or_assign(color, br);
				return br;
			} else {
				return v->second;
			}
		}

		HFONT GetFont(FormFont &font) {
			std::string idx = font.GetIdentity();
			auto v = Fonts_.find(idx);
			if (v == Fonts_.end()) {
				HFONT ft = font.Create(this->LogicalPixelY_);
				Fonts_.insert_or_assign(idx, ft);
				return ft;
			} else {
				return v->second;
			}
		}

		void Update(HDC hdc) {
			SetBkMode(hdc, TRANSPARENT);
			this->LastFontIdentity_ = "";
			this->LogicalPixelX_ = GetDeviceCaps(hdc, LOGPIXELSX);
			this->LogicalPixelY_ = GetDeviceCaps(hdc, LOGPIXELSY);
			this->Handle_ = hdc;
		}

		void FillRectangle(RECT r, HBRUSH brush) {
			this->Transform(r);
			FillRect(this->Handle_, &r, brush);
		}

		void DrawString(RECT r, const std::string &s, FormFont &font, TextAlignment alignment, Color color) {
			this->Transform(r);
			SetTextColor(this->Handle_, color);
			auto font_id = font.GetIdentity();
			if (font_id != LastFontIdentity_) {
				LastFontIdentity_ = font_id;
				SelectObject(this->Handle_, this->GetFont(font));
			}
			DrawText(this->Handle_, s.c_str(), s.length(), &r, (UINT)alignment);
		}
	};

	typedef class FormElement {
	protected:
		int Left_;
		int Top_;
		int Width_;
		int Height_;
		Color BackColor_;
		Color ForeColor_;
		HBRUSH BackBrushCache_;
		bool IsDirty_;
		FormElement *Parent_;

	public:
		Color GetBackColor() const { return BackColor_; }
		void SetBackColor(Color value) {
			if (value == BackColor_) {
				return;
			}
			if ((value < 0) && (Parent_ != nullptr)) {
				Parent_->IsDirty_ = true;
			}
			BackColor_ = value;
			BackBrushCache_ = NULL;
			IsDirty_ = true;
		}
		Color GetForeColor() const { return ForeColor_; }
		void SetForeColor(Color value) {
			ForeColor_ = value;
			IsDirty_ = true;
		}

		inline void SetDirty() {
			this->IsDirty_ = true;
		}

		inline bool IsDirty() {
			return this->IsDirty_;
		}

	protected:
		inline RECT GetRect(int base_x, int base_y) {
			RECT r;
			r.left = base_x + Left_;
			r.right = r.left + Width_;
			r.top = base_y + Top_;
			r.bottom = r.top + Height_;
			return r;
		}

	public:
		virtual void SetBounds(int left, int top, int width, int height) {
			if ((Left_ == left) && (Top_ == top) &&
				(Width_ == width) && (Height_ == height)) {
				return;
			}
			Left_ = left;
			Top_ = top;
			Width_ = width;
			Height_ = height;
			IsDirty_ = true;
			if (Parent_ != nullptr) {
				Parent_->IsDirty_ = true;
			}
			this->OnResize();
		}

		inline void SetParent(FormElement *el) {
			this->Parent_ = el;
		}

	public:
		virtual void OnDraw(FormGraphics &g, int base_x, int base_y) {
			if (!this->IsDirty_) {
				return;
			}
			if (BackColor_ > 0) {
				if (BackBrushCache_ == NULL) {
					BackBrushCache_ = g.GetBrush(BackColor_);
				}
				g.FillRectangle(GetRect(base_x, base_y), BackBrushCache_);
			}
		}

		virtual void OnResize() {
		}

		virtual void Invalidate() {
			this->IsDirty_ = true;
			if (this->Parent_ != nullptr) {
				this->Parent_->Invalidate();
			}
		}

		FormElement() :
			BackBrushCache_(NULL), BackColor_(-1), ForeColor_(0), IsDirty_(true),
			Left_(0), Top_(0), Width_(0), Height_(0), Parent_(nullptr) {
		}

		virtual ~FormElement() {
		}
	} *FormElementPtr;

	

	typedef class FormLabel : public FormElement {
	public:
		FormFont Font_;
		std::string Text_;
		TextAlignment Alignment_;

	public:
		FormLabel() : FormElement(), Text_(""), Alignment_(TextAlignment::TopLeft), Font_() {
		}

		virtual ~FormLabel() {
		}

	public:
		virtual void OnDraw(FormGraphics &g, int base_x, int base_y) {
			if (!IsDirty_) {
				return;
			}
			FormElement::OnDraw(g, base_x, base_y);
			g.DrawString(GetRect(base_x, base_y), this->Text_, Font_, this->Alignment_, this->ForeColor_);
		}

	public:
		std::string GetText() const { return Text_; }

		void SetText(const std::string &value) {
			Text_ = value;
			IsDirty_ = true;
		}

		TextAlignment GetAlignment() const { return Alignment_; }

		void SetAlignment(TextAlignment value) {
			Alignment_ = value;
			IsDirty_ = true;
		}

	} *FormLabelPtr;

	class FormContainer : public FormElement {
	protected:
		std::vector<FormElementPtr> Elements_;
	
	public:
		void AddElement(FormElementPtr el) {
			el->SetParent(this);
			this->Elements_.push_back(el);
			this->SetDirty();
		}

		bool RemoveElement(FormElementPtr el) {
			auto found = std::find(this->Elements_.begin(), this->Elements_.end(), el);
			if (found != this->Elements_.end()) {
				el->SetParent(nullptr);
				this->Elements_.erase(found);
				this->SetDirty();
				return true;
			} else {
				return false;
			}
		}

		void Clear() {
			for (auto el : Elements_) {
				delete el;
			}
			this->Elements_.clear();
		}

	public:
		virtual void OnDraw(FormGraphics &g, int base_x, int base_y) {
			if (this->IsDirty_) {
				FormElement::OnDraw(g, base_x, base_y);
			}
			for (auto el : Elements_) {
				el->OnDraw(g, base_x + Left_, base_y + Top_);
			}
		}

		FormContainer() : Elements_(), FormElement() {
		}

		virtual ~FormContainer() {
			Clear();
		}
	};
}
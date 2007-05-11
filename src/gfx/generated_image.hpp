//+----------------------------------------------------------------------------+
//| Description:  Magic Set Editor - Program to make Magic (tm) cards          |
//| Copyright:    (C) 2001 - 2007 Twan van Laarhoven                           |
//| License:      GNU General Public License 2 or later (see file COPYING)     |
//+----------------------------------------------------------------------------+

#ifndef HEADER_GFX_GENERATED_IMAGE
#define HEADER_GFX_GENERATED_IMAGE

// ----------------------------------------------------------------------------- : Includes

#include <util/prec.hpp>
#include <util/age.hpp>
#include <gfx/gfx.hpp>
#include <script/value.hpp>

DECLARE_INTRUSIVE_POINTER_TYPE(GeneratedImage);
DECLARE_POINTER_TYPE(SymbolVariation);
class Package;

// ----------------------------------------------------------------------------- : GeneratedImage

/// An image that is generated from a script.
/** The actual generation is independend of the script execution
 */
class GeneratedImage : public ScriptValue {
  public:
	/// Options for generating the image
	struct Options {
		Options(int width = 0, int height = 0, Package* package = nullptr, Package* symbol_package = nullptr, PreserveAspect preserve_aspect = ASPECT_STRETCH, bool saturate = false)
			: width(width), height(height), preserve_aspect(preserve_aspect), saturate(saturate), package(package), symbol_package(symbol_package)
		{}
		
		int            width, height;	///< Width to force the image to, or 0 to keep the width of the input
		PreserveAspect preserve_aspect;
		bool           saturate;
		Package* package;        ///< Package to load images from
		Package* symbol_package; ///< Package to load symbols from
	};
	
	/// Generate the image
	virtual Image generate(const Options&) const = 0;
	/// How must the image be combined with the background?
	virtual ImageCombine combine() const { return COMBINE_NORMAL; }
	/// Equality should mean that every pixel in the generated images is the same if the same options are used
	virtual bool operator == (const GeneratedImage& that) const = 0;
	inline  bool operator != (const GeneratedImage& that) const { return !(*this == that); }
	
	virtual ScriptType type() const;
	virtual String typeName() const;
};

// ----------------------------------------------------------------------------- : LinearBlendImage

/// An image generator that linearly blends two other images
class LinearBlendImage : public GeneratedImage {
  public:
	inline LinearBlendImage(const GeneratedImageP& image1, const GeneratedImageP& image2, double x1, double y1, double x2, double y2)
		: image1(image1), image2(image2), x1(x1), y1(y1), x2(x2), y2(y2)
	{}
	virtual Image generate(const Options& opt) const;
	virtual ImageCombine combine() const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	GeneratedImageP image1, image2;
	double x1, y1, x2, y2;
};

// ----------------------------------------------------------------------------- : MaskedBlendImage

/// An image generator that blends two other images using a third as a mask
class MaskedBlendImage : public GeneratedImage {
  public:
	inline MaskedBlendImage(const GeneratedImageP& light, const GeneratedImageP& dark, const GeneratedImageP& mask)
		: light(light), dark(dark), mask(mask)
	{}
	virtual Image generate(const Options& opt) const;
	virtual ImageCombine combine() const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	GeneratedImageP light, dark, mask;
};

// ----------------------------------------------------------------------------- : CombineBlendImage

/// An image generator that blends two other images using an ImageCombine function
class CombineBlendImage : public GeneratedImage {
  public:
	inline CombineBlendImage(const GeneratedImageP& image1, const GeneratedImageP& image2, ImageCombine image_combine)
		: image1(image1), image2(image2), image_combine(image_combine)
	{}
	virtual Image generate(const Options& opt) const;
	virtual ImageCombine combine() const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	GeneratedImageP image1, image2;
	ImageCombine image_combine;
};

// ----------------------------------------------------------------------------- : SetMaskImage

/// Change the alpha channel of an image
class SetMaskImage : public GeneratedImage {
  public:
	inline SetMaskImage(const GeneratedImageP& image, const GeneratedImageP& mask)
		: image(image), mask(mask)
	{}
	virtual Image generate(const Options& opt) const;
	virtual ImageCombine combine() const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	GeneratedImageP image, mask;
};

// ----------------------------------------------------------------------------- : SetCombineImage

/// Change the combine mode
class SetCombineImage : public GeneratedImage {
  public:
	inline SetCombineImage(const GeneratedImageP& image, ImageCombine image_combine)
		: image(image), image_combine(image_combine)
	{}
	virtual Image generate(const Options& opt) const;
	virtual ImageCombine combine() const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	GeneratedImageP image;
	ImageCombine image_combine;
};

// ----------------------------------------------------------------------------- : PackagedImage

/// Load an image from a file in a package
class PackagedImage : public GeneratedImage {
  public:
	inline PackagedImage(const String& filename)
		: filename(filename)
	{}
	virtual Image generate(const Options& opt) const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	String filename;
};

// ----------------------------------------------------------------------------- : BuiltInImage

/// Return a built in image
class BuiltInImage : public GeneratedImage {
  public:
	inline BuiltInImage(const String& name)
		: name(name)
	{}
	virtual Image generate(const Options& opt) const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	String name;
};

// ----------------------------------------------------------------------------- : SymbolToImage

/// Use a symbol as an image
class SymbolToImage : public GeneratedImage {
  public:
	inline SymbolToImage(const String& filename, Age age, const SymbolVariationP& variation)
		: filename(filename), age(age), variation(variation)
	{}
	virtual Image generate(const Options& opt) const;
	virtual bool operator == (const GeneratedImage& that) const;
  private:
	String           filename;
	Age              age; ///< Age the symbol was last updated
	SymbolVariationP variation;
};

// ----------------------------------------------------------------------------- : EOF
#endif
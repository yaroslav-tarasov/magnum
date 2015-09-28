/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "ImageData.h"

namespace Magnum { namespace Trade {

template<UnsignedInt dimensions> ImageData<dimensions>::ImageData(const PixelStorage storage, const PixelFormat format, const PixelType type, const typename DimensionTraits<dimensions, Int>::VectorType& size, Containers::Array<char>&& data): _compressed{false}, _s{storage}, _format{format}, _type{type}, _size{size}, _data{std::move(data)} {
    CORRADE_ASSERT(Implementation::imageDataSize(*this) <= _data.size(), "Trade::ImageData::ImageData(): bad image data size, got" << _data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
}

/* GCC 4.5 can't handle {} here */
template<UnsignedInt dimensions> ImageData<dimensions>::ImageData(const PixelFormat format, const PixelType type, const typename DimensionTraits<dimensions, Int>::VectorType& size, Containers::Array<char>&& data): _compressed{false}, _s(PixelStorage()), _format{format}, _type{type}, _size{size}, _data{std::move(data)} {
    CORRADE_ASSERT(Implementation::imageDataSize(*this) <= _data.size(), "Trade::ImageData::ImageData(): bad image data size, got" << _data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
}

#ifdef MAGNUM_BUILD_DEPRECATED
/* GCC 4.5 can't handle {} here */
template<UnsignedInt dimensions> ImageData<dimensions>::ImageData(const PixelFormat format, const PixelType type, const typename DimensionTraits<dimensions, Int>::VectorType& size, void* data): _compressed{false}, _s(PixelStorage()), _format{format}, _type{type}, _size{size}, _data{reinterpret_cast<char*>(data), Magnum::Implementation::imageDataSizeFor(format, type, size)} {
    /* No assert since data size is implicit */
}
#endif

template<UnsignedInt dimensions> PixelStorage ImageData<dimensions>::storage() const {
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::storage(): the image is compressed", {});
    return _s.storage();
}

template<UnsignedInt dimensions> PixelFormat ImageData<dimensions>::format() const {
    /* GCC 4.5 can't handle {} here */
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::format(): the image is compressed", PixelFormat());
    return _format;
}

template<UnsignedInt dimensions> PixelType ImageData<dimensions>::type() const {
    /* GCC 4.5 can't handle {} here */
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::type(): the image is compressed", PixelType());
    return _type;
}

#ifndef MAGNUM_TARGET_GLES
template<UnsignedInt dimensions> CompressedPixelStorage ImageData<dimensions>::compressedStorage() const {
    CORRADE_ASSERT(_compressed, "Trade::ImageData::compressedStorage(): the image is not compressed", {});
    return _s.compressedStorage();
}
#endif

template<UnsignedInt dimensions> CompressedPixelFormat ImageData<dimensions>::compressedFormat() const {
    /* GCC 4.5 can't handle {} here */
    CORRADE_ASSERT(_compressed, "Trade::ImageData::compressedFormat(): the image is not compressed", CompressedPixelFormat());
    return _compressedFormat;
}

template<UnsignedInt dimensions> std::size_t ImageData<dimensions>::pixelSize() const {
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::pixelSize(): the image is compressed", {});
    return PixelStorage::pixelSize(_format, _type);
}

template<UnsignedInt dimensions> std::tuple<std::size_t, typename DimensionTraits<dimensions, std::size_t>::VectorType, std::size_t> ImageData<dimensions>::dataProperties() const {
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::dataProperties(): the image is compressed", {});
    return Implementation::imageDataProperties<dimensions>(*this);
}

template<UnsignedInt dimensions> ImageData<dimensions>::operator ImageView<dimensions>()
#if !defined(CORRADE_GCC47_COMPATIBILITY) && !defined(CORRADE_MSVC2013_COMPATIBILITY)
const &
#else
const
#endif
{
    CORRADE_ASSERT(!_compressed, "Trade::ImageData::type(): the image is compressed", (ImageView<dimensions>{_s.storage(), _format, _type, _size}));
    return ImageView<dimensions>{_s.storage(), _format, _type, _size, _data};
}

template<UnsignedInt dimensions> ImageData<dimensions>::operator CompressedImageView<dimensions>()
#if !defined(CORRADE_GCC47_COMPATIBILITY) && !defined(CORRADE_MSVC2013_COMPATIBILITY)
const &
#else
const
#endif
{
    #ifndef MAGNUM_TARGET_GLES
    CORRADE_ASSERT(_compressed, "Trade::ImageData::type(): the image is not compressed", (CompressedImageView<dimensions>{_s.compressedStorage(), _compressedFormat, _size}));
    #else
    CORRADE_ASSERT(_compressed, "Trade::ImageData::type(): the image is not compressed", (CompressedImageView<dimensions>{_compressedFormat, _size}));
    #endif
    return CompressedImageView<dimensions>{
        #ifndef MAGNUM_TARGET_GLES
        _s.compressedStorage(),
        #endif
        _compressedFormat, _size, _data};
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template class MAGNUM_EXPORT ImageData<1>;
template class MAGNUM_EXPORT ImageData<2>;
template class MAGNUM_EXPORT ImageData<3>;
#endif

}}

/***************************************************************************
 *   Copyright (C) 2005 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _PDF_STREAM_H_
#define _PDF_STREAM_H_

#include "PdfDefines.h"

namespace PoDoFo {

typedef std::vector<EPdfFilter>            TVecFilters;
typedef TVecFilters::iterator              TIVecFilters;
typedef TVecFilters::const_iterator        TCIVecFilters;

class PdfName;
class PdfObject;

/** A PDF stream can be appended to any PdfObject
 *  and can contain abitrary data.
 *  Most of the time it will contain either drawing commands
 *  to draw onto a page or binary data like a font or an image.
 */
class PdfStream {
 public:
    /** Create a new PdfStream object which has a parent PdfObject.
     *  The stream will be deleted along with the parent.
     *  This constructor will be called by PdfObject::Stream() for you.
     *  \param pParent parent object
     */
    PdfStream( PdfObject* pParent );

    /** Create a copy of a PdfStream object
     *  \param rhs the object to clone
     */
    PdfStream( const PdfStream & rhs );

    ~PdfStream();

    /** Set a binary buffer as stream data
     *  The stream will be owned by the PdfObject and will be free'd 
     *  with it. The stream should be allocated using malloc and 
     *  not with new, as free is used instead of delete for freeing the memory.
     *  \param szBuffer buffer containing the stream data
     *  \param lLen length of the buffer
     *  \returns ErrOk
     */
    PdfError Set( char* szBuffer, long lLen );

    /** Set a text buffer as the streams contents.
     *  The string will be copied into a newly allocated buffer.
     *  \param pszString a zero terminated string buffer containing only ASCII text data
     *  \returns ErrOk on sucess
     */
    PdfError Set( const char* pszString );

    /** Append to the current stream contents. 
     *  \param pszString a zero terminated string buffer containing only ASCII text data
     *  \returns ErrOk on sucess
     */
    PdfError Append( const char* pszString ); 

    /** Append to the current stream contents. 
     *  \param pszString a buffer
     *  \param lLen length of the buffer
     *  \returns ErrOk on sucess
     */
    PdfError Append( const char* pszString, long lLen ); 

    /** Get a malloced buffer of the current stream.
     *  No filters will be applied to the buffer.
     *  The caller has to free the buffer.
     *  \param pBuffer pointer to the buffer
     *  \param lLen    pointer to the buffer length
     *  \returns ErrOk on success.
     */
    PdfError GetCopy( char** pBuffer, long* lLen ) const;

    /** Get a malloced buffer of the current stream which has been
     *  filtered by all filters as specified in the dictioniries
     *  filter key. The caller has to free the buffer.
     *  \param pBuffer pointer to the buffer
     *  \param lLen    pointer to the buffer length
     *  \returns ErrOk on success.
     */
    PdfError GetFilteredCopy( char** pBuffer, long* lLen ) const;

    /** Get a read-only handle to the currenct stream data.
     *  \returns a read-only handle to the streams data
     */
    inline const char* Get() const;

    /** Get the streams length
     *  \returns the length of the internal buffer
     *  \see Get()
     */
    inline long Length() const;

    /** Get the allocated size of the internal buffer
     *  \returns the size of the internal buffer which is normally larger
     *           than the length of the current buffer.
     */
    inline long Size() const;

    /** This function compresses any currently set stream
     *  using the FlateDecode algorithm. JPEG compressed streams
     *  will not be compressed again using this function.
     *  Entries to the filter dictionary will be added if necessary.
     *  \returns ErrOk on sucess
     */
    PdfError FlateDecode();

    /** Create a copy of a PdfStream object
     *  \param rhs the object to clone
     *  \returns a reference to this object
     */
    const PdfStream & operator=( const PdfStream & rhs );

 private:
    /** Reads the /Filters key from the current directory
     *  and adds every found filter to the vector.
     *  \param vecFilters add all filters to this vector
     *  \returns ErrOk on success
     */
    PdfError FillFilterList( TVecFilters & vecFilters ) const;

    /** Converts a filter name to the corresponding enum
     *  \param name of the filter without leading
     *  \param peFilter pointer to a filter enum where the result can be stored
     *  \returns ErrOk on success
     */
    static PdfError FilterNameToType( const PdfName & name, EPdfFilter* peFilter );

    /** Compress the current data using the FlateDecode algorithm
     *  Expects that all filters are setup correctly.
     *  \returns ErrOk on success
     */
    PdfError FlateDecodeStreamData();
    
 private:
    PdfObject*   m_pParent;

    char*        m_szStream;
    long         m_lLength;
    long         m_lSize;
};

const char* PdfStream::Get() const
    {
        return m_szStream;
    }

long PdfStream::Length() const
    {
        return m_lLength;
    }

long PdfStream::Size() const
    {
        return m_lSize;
    }

};

#endif // _PDF_STREAM_H_
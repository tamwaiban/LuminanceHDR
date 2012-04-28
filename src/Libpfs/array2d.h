/**
 * @file
 * @brief PFS library - general 2d array interface
 * 
 * This file is a part of Luminance HDR package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2003,2004 Rafal Mantiuk and Grzegorz Krawczyk
 * Copyright (C) 2011 Davide Anastasia
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ---------------------------------------------------------------------- 
 * 
 * @author Rafal Mantiuk, <mantiuk@mpi-sb.mpg.de>
 * $Id: array2d.h,v 1.1 2005/06/15 13:36:55 rafm Exp $
 *
 * @author Davide Anastasia <davideanastasia@users.sourceforge.net>
 *  This version is different then the one in the PFSTOOLS
 *  Classes Array2D and Array2DImpl are joined to create a clean class,
 *  which allows faster access to vector data, in order to create high performance
 *  routines. Old access functions are still available.
 *
 */

#ifndef ARRAY2D_H
#define ARRAY2D_H

namespace pfs
{ 
      /**
       * @brief Two dimensional array of floats
       *
       * This class holds 2 dimensional arrays of floats in column-major order.
       * Allows easy indexing and retrieving array dimensions. It offers an undirect
       * access to the data (using (x)(y) or (elem) ) or a direct access to the data
       * (using getRawData()).
       */
    class Array2D
    {
    private:
        float*  m_data;
        int     m_cols;
        int     m_rows;
        bool    m_is_data_owned;

        Array2D& operator = (const Array2D& other);
        Array2D(const Array2D& other);

    public:
        Array2D(int cols, int rows);
        Array2D(int cols, int rows, float* data);


        /**
         * Each implementing class should provide its own destructor.
         * It must be virtual to allow derived class to call their destructor as well.
         */
        virtual ~Array2D();

        /**
         * Access an element of the array for reading and
         * writing. Whether the given row and column are checked against
         * array bounds depends on an implementing class.
         *
         * Note, that if an Array2D object is passed as a pointer (what
         * is usually the case), to access its elements, you have to use
         * somewhat strange syntax: (*array)(row, column).
         *
         * @param col number of a column (x) within the range 0..(getCols()-1)
         * @param row number of a row (y) within the range 0..(getRows()-1)
         */
        float& operator()( int cols, int rows );

        /**
         * Access an element of the array for reading. Whether the given
         * row and column are checked against array bounds depends on an
         * implementing class.
         *
         * Note, that if an Array2D object is passed as a pointer (what
         * is usually the case), to access its elements, you have to use
         * somewhat strange syntax: (*array)(row, column).
         *
         * @param col number of a column (x) within the range 0..(getCols()-1)
         * @param row number of a row (y) within the range 0..(getRows()-1)
         */
        const float& operator()( int cols, int rows ) const;

        /**
         * Access an element of the array for reading and writing. This
         * is probably faster way of accessing elements than
         * operator(col, row). However there is no guarantee on the
         * order of elements as it usually depends on an implementing
         * class. The only assumption that can be make is that there are
         * exactly columns*rows elements and they are all unique.
         *
         * Whether the given index is checked against array bounds
         * depends on an implementing class.
         *
         * Note, that if an Array2D object is passed as a pointer (what
         * is usually the case), to access its elements, you have to use
         * somewhat strange syntax: (*array)(index).
         *
         * @param index index of an element within the range 0..(getCols()*getRows()-1)
         */
        float& operator()( int index );

        /**
         * Access an element of the array for reading. This
         * is probably faster way of accessing elements than
         * operator(col, row). However there is no guarantee on the
         * order of elements as it usually depends on an implementing
         * class. The only assumption that can be make is that there are
         * exactly columns*rows elements and they are all unique.
         *
         * Whether the given index is checked against array bounds
         * depends on an implementing class.
         *
         * Note, that if an Array2D object is passed as a pointer (what
         * is usually the case), to access its elements, you have to use
         * somewhat strange syntax: (*array)(index).
         *
         * @param index index of an element within the range 0..(getCols()*getRows()-1)
         */
        const float& operator()( int index ) const;

        /**
         * Get number of columns or, in case of an image, width.
         */
        int getCols() const;

        /**
         * Get number of rows or, in case of an image, height.
         */
        int getRows() const;

        /**
         * Direct access to the raw data
         */
        float*       getRawData();

        /**
         * Direct access to the raw data
         */
        const float* getRawData() const;
        
        /**
         * Reset the entire vector data to the value "value"
         */
        void reset(const float value = 0.0f);

        /**
         * Scale entire 2D array by "value"
         */
        void scale(const float value);
    };

    inline int Array2D::getCols() const { return m_cols; }
    inline int Array2D::getRows() const { return m_rows; }
    inline float* Array2D::getRawData() { return m_data; }
    inline const float* Array2D::getRawData() const { return m_data; }

    inline float& Array2D::operator()( int cols, int rows )
    {
        //assert( cols >= 0 && cols < m_cols );
        //assert( rows >= 0 && rows < m_rows );
        return m_data[ rows*m_cols + cols ];
    }

    inline const float& Array2D::operator()( int cols, int rows ) const
    {
        //assert( cols >= 0 && cols < m_cols );
        //assert( rows >= 0 && rows < m_rows );
        return m_data[ rows*m_cols + cols ];
    }

    inline float& Array2D::operator()( int index )
    {
        //assert( index >= 0 && index < m_rows*m_cols );
        return m_data[index];
    }

    inline const float& Array2D::operator()( int index ) const
    {
        //assert( index >= 0 && index <= m_rows*m_cols );
        return m_data[index];
    }

    void copyArray(const Array2D *from, Array2D *to);
    void copyArray(const Array2D *from, Array2D *to, int x_ul, int y_ul, int x_br, int y_br);

    void setArray(Array2D *array, const float value);
    void multiplyArray(Array2D *z, const Array2D *x, const Array2D *y);
    void divideArray(Array2D *z, const Array2D *x, const Array2D *y);
}

#endif // ARRAY2D_H

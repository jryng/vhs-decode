/************************************************************************

    isifilter.h

    ld-ldstoefm - LDS sample to EFM data processing
    Copyright (C) 2019 Simon Inns

    This file is part of ld-decode-tools.

    ld-ldstoefm is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#ifndef ISIFILTER_H
#define ISIFILTER_H

#include <QCoreApplication>
#include <QDebug>

// Note: The ISI filter is a raised cosine filter that acts to pulse-shape
// the EFM sample.  It is aligned based on T1/2 and should therefore
// sharpen the edges of the EFM pulses whilst attenuating interference.

class IsiFilter
{
public:
    IsiFilter();

    void floatIsiProcess(QByteArray &inputSample);
    void fixedIsiProcess(QByteArray &inputSample);

private:
    // Inter-Symbol Interference filter -------------------------------------------------------------------------------

    // M = Amount of over sampling
    // D = Number of symbols to be spanned by the impulse response
    // N = D * M (where both M and D are integers)
    //
    // Oversampling is 40,000,000 / 4,321,800 = 9.255402841
    // Best if D and M are integer multiples
    //
    // N = 9 * 9 = 81 taps

    // Generated by:   http://www-users.cs.york.ac.uk/~fisher/mkfilter
    //    filtertype 	= 	Raised Cosine
    //    samplerate 	= 	40000000
    //    corner 	= 	2160900
    //    beta 	= 	0.75
    //    impulselen 	= 	81
    //    racos 	= 	yes
    //    comp 	= 	no
    //    bits 	= 16
    //    logmin 	=

    /* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher */

    static const qint32 ceNZeros = 80; // 81 taps
    static constexpr qreal ceGain = 9.241973877e+00;
    qreal ceXv[ceNZeros+1];

    const qreal ceXcoeffs[ceNZeros+1] = {
        +0.0010986328, +0.0010681152, +0.0006713867, -0.0000305176,
        -0.0008239746, -0.0014953613, -0.0018005371, -0.0016479492,
        -0.0010070801, -0.0001220703, +0.0006408691, +0.0008850098,
        +0.0002746582, -0.0012817383, -0.0036010742, -0.0061340332,
        -0.0082092285, -0.0090332031, -0.0082092285, -0.0058593750,
        -0.0027770996, -0.0003662109, -0.0004882812, -0.0048522949,
        -0.0144348145, -0.0289611816, -0.0462341309, -0.0620727539,
        -0.0705566406, -0.0646667480, -0.0374755859, +0.0165710449,
        +0.1004028320, +0.2130126953, +0.3492431641, +0.4996948242,
        +0.6516418457, +0.7906494141, +0.9024047852, +0.9748840332,
        +0.9999694824, +0.9748840332, +0.9024047852, +0.7906494141,
        +0.6516418457, +0.4996948242, +0.3492431641, +0.2130126953,
        +0.1004028320, +0.0165710449, -0.0374755859, -0.0646667480,
        -0.0705566406, -0.0620727539, -0.0462341309, -0.0289611816,
        -0.0144348145, -0.0048522949, -0.0004882812, -0.0003662109,
        -0.0027770996, -0.0058593750, -0.0082092285, -0.0090332031,
        -0.0082092285, -0.0061340332, -0.0036010742, -0.0012817383,
        +0.0002746582, +0.0008850098, +0.0006408691, -0.0001220703,
        -0.0010070801, -0.0016479492, -0.0018005371, -0.0014953613,
        -0.0008239746, -0.0000305176, +0.0006713867, +0.0010681152,
        +0.0010986328
    };

    // Method prototypes ----------------------------------------------------------------------------------------------
    qreal floatIsiFilter(qreal inputSample);

    // Fixed point version (coeff scaled by 15 bits (32768))

    static const qint32 fpTaps = 81;
    qint16 fpXv[fpTaps];
    qint16 offset;

    qint16 fpCoeff[fpTaps] = {
        35, 34, 21, -1, -26, -48, -58, -53, -33,
        -3, 20, 29, 8, -42, -117, -200, -268,
        -295, -268, -192, -90, -11, -15, -158,
        -473, -948, -1515, -2033, -2311, -2118,
        -1227, 542, 3289, 6979, 11444, 16373,
        21352, 25908, 29570, 31944, 32766,
        31944, 29570, 25908, 21352, 16373,
        11444, 6979, 3289, 542, -1227, -2118,
        -2311, -2033, -1515, -948, -473, -158,
        -15, -11, -90, -192, -268, -295, -268,
        -200, -117, -42, 8, 29, 20, -3, -33,
        -53, -58, -48, -26, -1, 21, 34, 35
    };

    qint16 fixedIsiFilter(qint16 inputSample);
};

#endif // ISIFILTER_H
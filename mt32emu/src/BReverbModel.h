/* Copyright (C) 2003, 2004, 2005, 2006, 2008, 2009 Dean Beeler, Jerome Fisher
 * Copyright (C) 2011, 2012 Dean Beeler, Jerome Fisher, Sergey V. Mikayev
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MT32EMU_B_REVERB_MODEL_H
#define MT32EMU_B_REVERB_MODEL_H

namespace MT32Emu {

struct BReverbSettings {
	const Bit32u * const allpassSizes;
	const Bit32u * const combSizes;
	const Bit32u * const outLPositions;
	const Bit32u * const outRPositions;
	const Bit32u * const filterFactor;
	const Bit32u * const decayTimes;
	const Bit32u * const dryAmp;
	const Bit32u * const wetLevels;
	const Bit32u lpfAmp;
};

class RingBuffer {
protected:
	Bit16s *buffer;
	const Bit32u size;
	Bit32u index;

public:
	RingBuffer(const Bit32u size);
	virtual ~RingBuffer();
	Bit32s next();
	bool isEmpty() const;
	void mute();
};

class AllpassFilter : public RingBuffer {
public:
	AllpassFilter(const Bit32u size);
	Bit32s process(const Bit32s in);
};

class CombFilter : public RingBuffer {
	Bit32u feedbackFactor;
	Bit32u filterFactor;

public:
	CombFilter(const Bit32u size);
	void process(const Bit32s in, const Bit32u lpfMask);
	Bit32s getOutputAt(const Bit32u outIndex) const;
	void setFeedbackFactor(const Bit32u useFeedbackFactor);
	void setFilterFactor(const Bit32u useFilterFactor);
};

class BReverbModel : public ReverbModel {
	AllpassFilter **allpasses;
	CombFilter **combs;

	const BReverbSettings &currentSettings;
	Bit32u dryAmp;
	Bit32u wetLevel;
	void mute();

public:
	BReverbModel(const ReverbMode mode);
	~BReverbModel();
	void open(unsigned int sampleRate);
	void close();
	void setParameters(Bit8u time, Bit8u level);
	void process(const float *inLeft, const float *inRight, float *outLeft, float *outRight, unsigned long numSamples);
	bool isActive() const;
};

}

#endif
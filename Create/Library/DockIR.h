/*
 *  DockIR.h
 *
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DOCKIR_H_
#define DOCKIR_H_

class DockIR {

private:
	bool _red;
	bool _green;
	bool _force;
	bool _reserved;
	int _signal;

public:
	DockIR() {
		_red = false;
		_green = false;
		_force = false;
		_reserved = false;
		_signal = 0;
	}

	DockIR(int irsignal) {
		_red = 		irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_FORCEFIELD ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY_FORCEFIELD;
		_green = 	irsignal == COIL::IR_SIGNAL_HOMEBASE_GREENBUOY_FORCEFIELD ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_GREENBUOY ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY_FORCEFIELD;
		_force = 	irsignal == COIL::IR_SIGNAL_HOMEBASE_FORCEFIELD ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_FORCEFIELD ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_GREENBUOY_FORCEFIELD ||
					irsignal == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY_FORCEFIELD;
		_reserved = 	irsignal == COIL::IR_SIGNAL_HOMEBASE_RESERVED;
		_signal = irsignal;
	}

	bool red() { return _red; }
	bool green() { return _green; }
	bool force() { return _force; }
	bool forcebothbeam() { return _red && _green && _force;}
	bool unknown() { return !_force && !_red && !_green; }
	bool beam() { return _red || _green; }
	bool nobeam() { return !_red && !_green; }
	bool bothbeam() { return _red && _green; }
	bool redgreen() { return _red && _green; }
	bool onlygreen() { return !_red && _green; }
	bool onlyred() { return _red && !_green; }
	bool reserved() { return _reserved; }
	int signal() { return _signal; }
};

#endif /* DOCKIR_H_ */

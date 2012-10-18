/*
 *	bitz-server, An ICAP server implementation in C++
 *	Copyright (C) 2012	Uditha Atukorala
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"
#include <libconfig.h++>

namespace bitz {

	Config::Config() {

		// initialise config_t values
		this->_config.port            = 4040;
		this->_config.log_file        = "/dev/null";
		this->_config.log_category    = "bitz";

	}

	Config::~Config() { }

	const config_t &Config::initialise( std::string config_file ) {

		libconfig::Config config;

		try {
			config.readFile( config_file.c_str() );
		} catch( const libconfig::FileIOException &e ) {
			// TODO
		}

		try {
			config.lookupValue( "port", this->_config.port );
			config.lookupValue( "log_file", this->_config.log_file );
			config.lookupValue( "log_category", this->_config.log_category );
		} catch( const libconfig::SettingNotFoundException &e ) {
			// TODO
		}

		return this->_config;

	}

	const config_t &Config::getConfig() {
		return this->_config;
	}

} // end of namespace bitz


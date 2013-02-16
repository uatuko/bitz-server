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

#include <iostream>


namespace bitz {

	Config::Config() {

		// initialise config_t values
		_config.port            = 1344;
		_config.log_file        = "/dev/null";
		_config.log_category    = "bitz";

		// defaults
		_lconfig = NULL;

	}

	Config::~Config() {

		// cleanup
		delete _lconfig;

	}


	const config_t &Config::initialise( const std::string &config_file ) {

		libconfig::Config * config;
		config = new libconfig::Config;

		try {
			config->readFile( config_file.c_str() );
		} catch( const libconfig::FileIOException &ex ) {
			// TODO
		} catch( const libconfig::ParseException &pex ) {
			// FIXME
			std::cerr << "Parse error at " << pex.getFile()
					<< ":" << pex.getLine() << " - " << pex.getError() << std::endl;
		}

		try {

			// read core configs
			config->lookupValue( "port", _config.port );
			config->lookupValue( "log_file", _config.log_file );
			config->lookupValue( "log_category", _config.log_category );

			// cache configs
			_lconfig = config;

		} catch( const libconfig::SettingNotFoundException &e ) {
			// TODO
		}

		return _config;

	}


	const config_t &Config::configs() {
		return _config;
	}


	const std::string Config::module_config( const std::string &module, const std::string &config ) throw() {

		std::string config_value = "";

		if ( _lconfig->exists( "modules" ) ) {

			try {
				libconfig::Setting &setting = _lconfig->lookup( std::string( "modules." ).append( module ) );
				setting.lookupValue( config, config_value );
			} catch( const libconfig::SettingNotFoundException &e ) {
				// TODO: log errors ??
			}

		}

		return config_value;

	}

} /* end of namespace bitz */


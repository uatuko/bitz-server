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
		this->_config.port            = 1344;
		this->_config.log_file        = "/dev/null";
		this->_config.log_category    = "bitz";

		// defaults
		this->_lconfig = NULL;

	}

	Config::~Config() { }


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
			config->lookupValue( "port", this->_config.port );
			config->lookupValue( "log_file", this->_config.log_file );
			config->lookupValue( "log_category", this->_config.log_category );

			// cache configs
			this->_lconfig = config;

		} catch( const libconfig::SettingNotFoundException &e ) {
			// TODO
		}

		return this->_config;

	}


	const config_t &Config::configs() {
		return this->_config;
	}


	const libconfig::Setting * Config::module_configs( const std::string & module ) {

		libconfig::Setting * setting;
		setting = NULL;

		if ( this->_lconfig->exists( "module" ) ) {

			try {
				setting = &this->_lconfig->lookup( std::string( "module." ).append( module ) );
			} catch( const libconfig::SettingNotFoundException &e ) {
				// TODO
			}

		}

		return setting;

	}

} /* end of namespace bitz */


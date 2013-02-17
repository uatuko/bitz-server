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
#include "logger.h"

#include <iostream>


namespace bitz {

	Config::Config() {

		// initialise config_t values
		_config.port            = 1344;
		_config.log_file        = "/dev/null";
		_config.log_category    = "bitz";
		_config.req_handlers_count = 0;
		_config.req_handlers       = NULL;

		// defaults
		_lconfig = NULL;

	}

	Config::~Config() {

		// cleanup
		delete _lconfig;

		for (int i = 0; i < _config.req_handlers_count ; i++ ) {
			if ( _config.req_handlers[i].modules_count > 0 ) {
				delete [] _config.req_handlers[i].modules;
			}
		}

		if ( _config.req_handlers != NULL ) {
			delete [] _config.req_handlers;
		}

	}


	const config_t &Config::initialise( const std::string &config_file ) {

		libconfig::Config * config;
		config = new libconfig::Config;

		try {
			config->readFile( config_file.c_str() );
		} catch( const libconfig::FileIOException &ex ) {
			std::cerr << "[config] failed to read config file: " << ex.what() << std::endl;
		} catch( const libconfig::ParseException &pex ) {
			std::cerr << "[config] parse error at " << pex.getFile()
					<< ":" << pex.getLine() << " - " << pex.getError() << std::endl;
		}

		try {

			// read core configs
			config->lookupValue( "port", _config.port );
			config->lookupValue( "log_file", _config.log_file );
			config->lookupValue( "log_category", _config.log_category );

		} catch( const libconfig::SettingNotFoundException &e ) {
			std::cerr << "[config] failed to load core configs, "
					<< e.getPath() << " : " << e.what() << std::endl;
		}

		// cache configs
		_lconfig = config;

		// read other configs
		read_req_handler_configs();

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
				std::cerr << "[config] " << e.getPath() << " : " << e.what() << std::endl;
			}

		} else {
			std::cout << "[config] 'modules' configs not found" << std::endl;
		}

		return config_value;

	}


	void Config::read_req_handler_configs() throw() {

		int i, j;
		std::string s;

		std::cout << "[config] looking for req_handlers... ";
		if ( _lconfig->exists( "req_handlers" ) ) {

			std::cout << "found ";

			libconfig::Setting &req_handlers = _lconfig->lookup( "req_handlers" );
			_config.req_handlers_count       = req_handlers.getLength();
			_config.req_handlers             = new req_handlers_config_t[_config.req_handlers_count];

			std::cout << "(" << _config.req_handlers_count << ")" << std::endl;

			try {

				// read request handler configs
				for ( i = 0; i < _config.req_handlers_count ; i++ ) {
					_config.req_handlers[i].name       = (const char *) req_handlers[i]["handler"];
					_config.req_handlers[i].class_name = (const char *) req_handlers[i]["class"];

					// read request handler modules config
					std::cout << "[config] looking for " << _config.req_handlers[i].name << " modules... ";
					if ( req_handlers[i].exists( "modules" ) ) {
						std::cout << "found ";

						_config.req_handlers[i].modules_count   = req_handlers[i]["modules"].getLength();
						_config.req_handlers[i].modules         = new modules_config_t[_config.req_handlers[i].modules_count];

						std::cout << "(" << _config.req_handlers[i].modules_count << ")" << std::endl;
						for ( j = 0; j < _config.req_handlers[i].modules_count; j++ ) {
							_config.req_handlers[i].modules[j].name   = (const char *) req_handlers[i]["modules"][j]["name"];
							_config.req_handlers[i].modules[j].module = (const char *) req_handlers[i]["modules"][j]["module"];
						}
					} else {
						std::cout << "not found" << std::endl;
					}
				}

			} catch ( const libconfig::SettingNotFoundException &ex ) {
				std::cerr << "[config] Error: " << ex.getPath() << ex.what() << std::endl;
			}

		} else {
			std::cout << "not found" << std::endl;
		}

	}

} /* end of namespace bitz */


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

#include <cstdlib>


namespace bitz {

	Config::Config() {

		// initialise config_t values
		_config.port            = 1344;
		_config.pid_file        = "/dev/null";
		_config.log_file        = "/dev/null";
		_config.log_category    = "bitz";
		_config.req_handlers_count = 0;
		_config.req_handlers       = NULL;

		_config.max_workers         = 0;
		_config.max_worker_requests = 0;

		_config.comm_timeout = 0;

		// defaults
		_lconfig = NULL;

		// logger
		_logger = spdlog::get( "bitz-server" );

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
		} catch ( const libconfig::FileIOException &ex ) {
			_logger->error( "[config] failed to read config file: {}, exception: {}",  config_file, ex.what() );
			exit( EXIT_FAILURE );
		} catch ( const libconfig::ParseException &pex ) {
			_logger->error( "[config] parse error at {}:{} - {}", pex.getFile(), pex.getLine(), pex.getError() );
			exit( EXIT_FAILURE );
		}

		try {

			// read core configs
			config->lookupValue( "port", _config.port );
			config->lookupValue( "pid_file", _config.pid_file );
			config->lookupValue( "log_file", _config.log_file );
			config->lookupValue( "log_category", _config.log_category );

			config->lookupValue( "max_workers", _config.max_workers );
			config->lookupValue( "max_worker_requests", _config.max_worker_requests );

			config->lookupValue( "comm_timeout", _config.comm_timeout );

		} catch ( const libconfig::SettingNotFoundException &e ) {
			_logger->error( "[config] failed to load core configs, {} : {}", e.getPath(), e.what() );
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
			} catch ( const libconfig::SettingNotFoundException &e ) {
				_logger->error( "[config] {} : {}", e.getPath(), e.what() );
			}

		} else {
			_logger->info( "[config] 'modules' configs not found" );
		}

		return config_value;

	}


	void Config::read_req_handler_configs() throw() {

		int i, j;
		std::string s;

		_logger->trace( "[config] looking for req_handlers" );
		if ( _lconfig->exists( "req_handlers" ) ) {

			libconfig::Setting &req_handlers = _lconfig->lookup( "req_handlers" );
			_config.req_handlers_count       = req_handlers.getLength();
			_config.req_handlers             = new req_handlers_config_t[_config.req_handlers_count];

			_logger->debug( "[config] found {} request handlers", _config.req_handlers_count );

			try {

				// read request handler configs
				for ( i = 0; i < _config.req_handlers_count ; i++ ) {
					_config.req_handlers[i].name       = (const char *) req_handlers[i]["handler"];
					_config.req_handlers[i].class_name = (const char *) req_handlers[i]["class"];

					// read request handler modules config
					_logger->debug( "[config] looking for {} modules", _config.req_handlers[i].name );
					if ( req_handlers[i].exists( "modules" ) ) {

						_config.req_handlers[i].modules_count   = req_handlers[i]["modules"].getLength();
						_config.req_handlers[i].modules         = new modules_config_t[_config.req_handlers[i].modules_count];

						_logger->debug( "[config] found {} modules for {}", _config.req_handlers[i].modules_count, _config.req_handlers[i].name );
						for ( j = 0; j < _config.req_handlers[i].modules_count; j++ ) {
							_config.req_handlers[i].modules[j].name   = (const char *) req_handlers[i]["modules"][j]["name"];
							_config.req_handlers[i].modules[j].module = (const char *) req_handlers[i]["modules"][j]["module"];
						}
					} else {
						_logger->info( "[config] no modules found for {}", _config.req_handlers[i].name );
					}
				}

			} catch ( const libconfig::SettingNotFoundException &ex ) {
				_logger->error( "[config] {} : {}", ex.getPath(), ex.what() );
			}

		} else {
			_logger->info( "[config] no request handlers found" );
		}

	}

} /* end of namespace bitz */


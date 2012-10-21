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

#include "logger.h"

#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

namespace bitz {

	Logger::Logger( std::string log_file, std::string category ) {
		this->LOGGER = NULL;
		this->initialise( log_file, category );
	}

	Logger::~Logger() {
		this->LOGGER->debug( "closing down logger" );
	}

	void Logger::initialise( std::string log_file, std::string category ) {

		this->LOGGER = &log4cpp::Category::getInstance( category );

		// setting up appender, layout and category
		log4cpp::Appender * log_appender    = new log4cpp::FileAppender( "FileAppender", log_file );
		log4cpp::PatternLayout * log_layout = new log4cpp::PatternLayout();
		log_layout->setConversionPattern( "%d %p %c %x: %m%n" );

	    log_appender->setLayout( log_layout );
	    this->LOGGER->setAppender( log_appender );
	    this->LOGGER->setPriority( this->getPriorityValue( "DEBUG" ) );

	    this->LOGGER->debug( "logger initialised, log_file: " + log_file );

	}

	int Logger::getPriorityValue( const std::string &priority ) {
		return ( log4cpp::Priority::getPriorityValue( priority ) );
	}

	void Logger::log( log4cpp::Priority::Value priority, const std::string &message ) {
		this->LOGGER->log( priority, message );
	}

	void Logger::fatal( const std::string& message ) {
		this->LOGGER->fatal( message );
	}

	void Logger::emerg( const std::string& message ) {
		this->LOGGER->emerg( message );
	}

	void Logger::alert( const std::string& message ) {
		this->LOGGER->alert( message );
	}

	void Logger::crit( const std::string& message ) {
		this->LOGGER->crit( message );
	}

	void Logger::error( const std::string& message ) {
		this->LOGGER->error( message );
	}

	void Logger::warn( const std::string& message ) {
		this->LOGGER->warn( message );
	}

	void Logger::notice( const std::string& message ) {
		this->LOGGER->notice( message );
	}

	void Logger::info( const std::string& message ) {
		this->LOGGER->info( message );
	}

	void Logger::debug( const std::string& message ) {
		this->LOGGER->debug( message );
	}

} // end of namespace bitz


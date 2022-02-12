/*!==========================================================================
 * \file
 *   Program:       auth-grpc
 *   File:          arg-list.h
 *   Created:       02/11/2022
 *   Author:        Vitaly Bulganin
 *   Description:
 *   Comments:
 *
-----------------------------------------------------------------------------
 *
 * History:
 *
===========================================================================*/
#pragma once
//-------------------------------------------------------------------------//
#ifndef	__ARG_LIST_H_972708CB_F0C8_422C_95F3_D14DEB1039B5__
#define	__ARG_LIST_H_972708CB_F0C8_422C_95F3_D14DEB1039B5__
//-------------------------------------------------------------------------//
#include <map>
#include <vector>
#include <string>
#include <ostream>
//-------------------------------------------------------------------------//
namespace auth {
	namespace common {
//-------------------------------------------------------------------------//
		class arglist final {
			using arguments_t = std::vector<std::string>;
			using params_t = std::map<std::string, std::string>;
			using pair_t = params_t::value_type;
			//!<
			friend std::ostream & operator<<(std::ostream & stream, const arglist & args);
			//!< Keeps a set of parameters.
			params_t params;
			//!< Keeps a list of arguments.
			arguments_t args;

		public:
			arglist(const arglist &) = delete;
			arglist(const arglist &&) = delete;
			arglist & operator=(const arglist &) = delete;

		public:
			using argument_const_iterator = arguments_t::const_iterator;
			using param_const_iterator = params_t::const_iterator;

		protected:
			//!< Constructs
			void build(int argc, char * argv[]);

		public:
			/**
			 * Constructor.
			 * @param argc [in] - A count of args.
			 * @param argv [in] - A list of args.
			 */
			arglist(int argc, char * argv[]);

			/**
			 * Destructor.
			 * @throw None.
			 */
			~arglist() noexcept = default;

			/**
			 * Checks a key on existence.
			 * @param key [in] - A key.
			 * @return true, if the key is in the args, otherwise false.
			 * @throw None.
			 */
			auto contains(const std::string & key) const noexcept -> bool;

			/**
			 * Gets the first argument.
			 * @return The first argument.
			 * @throw None.
			 */
			auto begin() const noexcept -> argument_const_iterator {
                return this->args.begin();
            }

			/**
			 * Gets the first parameter.
			 * @return The first parameter.
			 * @throw None.
			 */
			auto begin_param() const noexcept -> param_const_iterator {
                return this->params.begin();
            }

			/**
			 * Gets the last argument.
			 * @return The last argument.
			 * @throw None.
			 */
			auto end() const noexcept -> argument_const_iterator {
                return this->args.end();
            }

			/**
			 * Gets the last parameter.
			 * @return The last parameters.
			 * @throw None.
			 */
			auto end_param() const noexcept -> param_const_iterator {
                return this->params.end();
            }

			/**
			 * Checks a list of arguments on empty.
			 * @return true, if the list is empty, otherwise false.
			 * @throw None.
			 */
			auto empty() const noexcept -> bool {
                return this->args.empty();
            }

			/**
			 * Gets a count of arguments.
			 * @return A count of arguments.
			 * @throw None.
			 */
			auto size() const noexcept -> arguments_t::size_type {
                return this->args.size();
            }

			// Operators.
		public:
		    //!< Gets a value of parameter by its key.
			auto operator[](const std::string & key) const noexcept -> const std::string &;

			/**
			 * Gets an argument by its index.
			 * @throw std::out_of_range - The index is out of range.
			 */
			auto operator[](int index) const -> const std::string &;
		};
//-------------------------------------------------------------------------//
        auto operator<<(std::ostream & stream, const arglist & args) -> std::ostream &;
//-------------------------------------------------------------------------//
	}; // namespace common
}; // namespace auth
//-------------------------------------------------------------------------//
#endif // __ARG_LIST_H_972708CB_F0C8_422C_95F3_D14DEB1039B5__

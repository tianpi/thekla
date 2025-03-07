// Module:  Log4CPLUS

// File:    filter.h

// Created: 5/2003

// Author:  Tad E. Smith

//

//

// Copyright (C) Tad E. Smith  All rights reserved.

//

// This software is published under the terms of the Apache Software

// License version 1.1, a copy of which has been included with this

// distribution in the LICENSE.APL file.

//



/** @file 

 * This header defines Filter and all of it's subclasses. */



#ifndef LOG4CPLUS_SPI_FILTER_HEADER_

#define LOG4CPLUS_SPI_FILTER_HEADER_



#include <log4cplus/config.h>

#include <log4cplus/helpers/pointer.h>

#include <log4cplus/helpers/property.h>

#include <log4cplus/spi/loggingevent.h>





namespace log4cplus {

    namespace spi {





        enum FilterResult { DENY, /**< The log event must be dropped immediately 

                                   *  without consulting with the remaining 

                                   *  filters, if any, in the chain. */

                            NEUTRAL, /**< This filter is neutral with respect to

                                      *  the log event; the remaining filters, if 

                                      *  if any, should be consulted for a final 

                                      *  decision. */

                            ACCEPT /**< The log event must be logged immediately 

                                    *  without consulting with the remaining 

                                    *  filters, if any, in the chain. */

                          };



        // Forward Declarations

        class Filter;





        /**

         * This method is used to filter an InternalLoggingEvent.

         *

         * <p>Note: <code>filter</code> can be NULL.

         */

        LOG4CPLUS_EXPORT FilterResult checkFilter(const Filter* filter, 

                                                  const InternalLoggingEvent& event);



        typedef helpers::SharedObjectPtr<Filter> FilterPtr;





        /**

         * Users should extend this class to implement customized logging

         * event filtering. Note that the {@link Logger} and {@link

         * Appender} classes have built-in filtering rules. It is suggested 

         * that you first use and understand the built-in rules before rushing

         * to write your own custom filters.

         *

         * <p>This abstract class assumes and also imposes that filters be

         * organized in a linear chain. The {@link #decide

         * decide(LoggingEvent)} method of each filter is called sequentially,

         * in the order of their addition to the chain.

         *

         * <p>If the value {@link #DENY} is returned, then the log event is

         * dropped immediately without consulting with the remaining

         * filters. 

         *

         * <p>If the value {@link #NEUTRAL} is returned, then the next filter

         * in the chain is consulted. If there are no more filters in the

         * chain, then the log event is logged. Thus, in the presence of no

         * filters, the default behaviour is to log all logging events.

         *

         * <p>If the value {@link #ACCEPT} is returned, then the log

         * event is logged without consulting the remaining filters. 

         *

         * <p>The philosophy of log4cplus filters is largely inspired from the

         * Linux ipchains. 

         */

        class LOG4CPLUS_EXPORT Filter : public log4cplus::helpers::SharedObject {

        public:

          // ctor and dtor

            Filter();

            virtual ~Filter();



          // Methods

            /**

             * Appends <code>filter</code> to the end of this filter chain.

             */

            void appendFilter(FilterPtr filter);



            /**     

             * <p>If the decision is <code>DENY</code>, then the event will be

             * dropped. If the decision is <code>NEUTRAL</code>, then the next

             * filter, if any, will be invoked. If the decision is ACCEPT then

             * the event will be logged without consulting with other filters in

             * the chain.

             *

             * @param event The LoggingEvent to decide upon.

             * @param decision The decision of the filter.  

             */

            virtual FilterResult decide(const InternalLoggingEvent& event) const = 0;



          // Data

            /**

             * Points to the next filter in the filter chain.

             */

            FilterPtr next;

        };

  





        /**

         * This filter drops all logging events.

         *

         * <p>You can add this filter to the end of a filter chain to

         * switch from the default "accept all unless instructed otherwise"

         * filtering behaviour to a "deny all unless instructed otherwise"

         * behaviour.

         */

        class LOG4CPLUS_EXPORT DenyAllFilter : public Filter {

        public:

            /**

             * Always returns the {@link DENY} regardless of the 

             * {@link InternalLoggingEvent} parameter.

             */

            virtual FilterResult decide(const InternalLoggingEvent& event) const;

        };





        /**

         * This is a very simple filter based on LogLevel matching.

         *

         * <p>The filter admits two options <b>LogLevelToMatch</b> and

         * <b>AcceptOnMatch</b>. If there is an exact match between the value

         * of the LogLevelToMatch option and the LogLevel of the {@link

         * LoggingEvent}, then the {@link #decide} method returns 

         * {@link #ACCEPT} in case the <b>AcceptOnMatch</b> option value is set

         * to <code>true</code>, if it is <code>false</code> then {@link #DENY}

         * is returned. If there is no match, {@link #NEUTRAL} is returned.

         */

        class LOG4CPLUS_EXPORT LogLevelMatchFilter : public Filter {

        public:

            LogLevelMatchFilter();

            LogLevelMatchFilter(const log4cplus::helpers::Properties& p);



            /**

             * Return the decision of this filter.

             *

             * Returns {@link #NEUTRAL} if the <b>LogLevelToMatch</b>

             * option is not set or if there is no match.  Otherwise, if

             * there is a match, then the returned decision is {@link #ACCEPT}

             * if the <b>AcceptOnMatch</b> property is set to <code>true</code>. 

             * The returned decision is {@link #DENY} if the <b>AcceptOnMatch</b>

             * property is set to <code>false</code>.

             */

            virtual FilterResult decide(const InternalLoggingEvent& event) const;



        private:

          // Methods

            void init();



          // Data

            /** Do we return ACCEPT when a match occurs. Default is <code>true</code>. */

            bool acceptOnMatch;

            LogLevel logLevelToMatch;

        };







        /**

         * This is a very simple filter based on LogLevel matching, which can be

         * used to reject messages with LogLevels outside a certain range.

         *

         * <p>The filter admits three options <b>LogLevelMin</b>, <b>LogLevelMax</b>

         * and <b>AcceptOnMatch</b>.

         *

         * <p>If the LogLevel of the Logging event is not between Min and Max

         * (inclusive), then {@link #DENY} is returned.

         *

         * <p> If the Logging event LogLevel is within the specified range, then if

         * <b>AcceptOnMatch</b> is true, {@link #ACCEPT} is returned, and if

         * <b>AcceptOnMatch</b> is false, {@link #NEUTRAL} is returned.

         *

         * <p>If <code>LogLevelMin</code> is not defined, then there is no

         * minimum acceptable LogLevel (ie a LogLevel is never rejected for

         * being too "low"/unimportant).  If <code>LogLevelMax</code> is not

         * defined, then there is no maximum acceptable LogLevel (ie a

         * LogLevel is never rejected for beeing too "high"/important).

         *

         * <p>Refer to the {@link

         * Appender#setThreshold setThreshold} method

         * available to <code>all</code> appenders for a more convenient way to

         * filter out events by LogLevel.

         */

        class LOG4CPLUS_EXPORT LogLevelRangeFilter : public Filter {

        public:

          // ctors

            LogLevelRangeFilter();

            LogLevelRangeFilter(const log4cplus::helpers::Properties& p);



            /**

             * Return the decision of this filter.

             */

            virtual FilterResult decide(const InternalLoggingEvent& event) const;



        private:

          // Methods

            void init();



          // Data

            /** Do we return ACCEPT when a match occurs. Default is <code>true</code>. */

            bool acceptOnMatch;

            LogLevel logLevelMin;

            LogLevel logLevelMax;

        };







        /**

         * This is a very simple filter based on string matching.

         *

         * <p>The filter admits two options <b>StringToMatch</b> and

         * <b>AcceptOnMatch</b>. If there is a match between the value of the

         * StringToMatch option and the message of the Logging event,

         * then the {@link #decide} method returns {@link #ACCEPT} if

         * the <b>AcceptOnMatch</b> option value is true, if it is false then

         * {@link #DENY} is returned. If there is no match, {@link #NEUTRAL} 

         * is returned.

         */

        class LOG4CPLUS_EXPORT StringMatchFilter : public Filter {

        public:

          // ctors

            StringMatchFilter();

            StringMatchFilter(const log4cplus::helpers::Properties& p);



            /**

             * Returns {@link #NEUTRAL} is there is no string match.

             */

            virtual FilterResult decide(const InternalLoggingEvent& event) const;



        private:

          // Methods

            void init();



          // Data

            /** Do we return ACCEPT when a match occurs. Default is <code>true</code>. */

            bool acceptOnMatch;

            log4cplus::tstring stringToMatch;

        };



    } // end namespace spi

} // end namespace log4cplus



#endif /* LOG4CPLUS_SPI_FILTER_HEADER_ */






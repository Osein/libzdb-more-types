/*
 * Copyright (C) Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 *
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.
 */


#ifndef RESULTSET_INCLUDED
#define RESULTSET_INCLUDED
//<< Protected methods
#include "ResultSetDelegate.h"
//>> End Protected methods
#include "SQLDateTime.h"


/**
 * A <b>ResultSet</b> represents a database result set. A ResultSet is
 * created by executing a SQL SELECT statement using either 
 * Connection_executeQuery() or PreparedStatement_executeQuery().
 *
 * A ResultSet maintains a cursor pointing to its current row of data. 
 * Initially the cursor is positioned before the first row. 
 * ResultSet_next() moves the cursor to the next row, and because 
 * it returns false when there are no more rows in the ResultSet object,
 * it can be used in a while loop to iterate through the result set.  
 * A ResultSet object is not updatable and has a cursor that moves forward
 * only. Thus, you can iterate through it only once and only from the first
 * row to the last row.
 *
 * The ResultSet interface provides getter methods for retrieving
 * column values from the current row. Values can be retrieved using
 * either the index number of the column or the name of the column. In
 * general, using the column index will be more efficient. <i>Columns
 * are numbered from 1.</i> 
 *
 * Column names used as input to getter methods are case sensitive.
 * When a getter method is called with a column name and several
 * columns have the same name, the value of the first matching column
 * will be returned. The column name option is designed to be used
 * when column names are used in the SQL query that generated the
 * result set. For columns that are NOT explicitly named in the query,
 * it is best to use column indices.
 *
 * <h3>Example</h3>
 * The following examples demonstrate how to obtain a ResultSet and 
 * how to get values from the set:
 * <pre>
 * ResultSet_T r = Connection_executeQuery(con, "SELECT ssn, name, picture FROM employees");
 * while (ResultSet_next(r)) 
 * {
 *      int ssn = ResultSet_getIntByName(r, "ssn");
 *      const char *name =  ResultSet_getStringByName(r, "name");
 *      int pictureSize;
 *      const void *picture = ResultSet_getBlobByName(r, "picture", &pictureSize);
 *      [..]
 * }
 * </pre>
 * Here is another example where a generated result is selected and printed:
 * <pre>
 * ResultSet_T r = Connection_executeQuery(con, "SELECT count(*) FROM users");
 * printf("Number of users: %s\n", ResultSet_next(r) ? ResultSet_getString(r, 1) : "no users");
 * </pre>
 *
 * <h3>Automatic type conversions</h3>
 * A ResultSet store values internally as bytes and convert values 
 * on-the-fly to numeric types when requested, such as when ResultSet_getInt()
 * or one of the other numeric get-methods are called. In the above example, 
 * even if <i>count(*)</i> returns a numeric value, we can use 
 * ResultSet_getString() to get the number as a string or if we choose, we can use
 * ResultSet_getInt() to get the value as an integer. In the latter case, note
 * that if the column value cannot be converted to a number, an SQLException is thrown.
 *
 * <i>A ResultSet is reentrant, but not thread-safe and should only be used by one thread (at the time).</i>
 *
 * @see Connection.h PreparedStatement.h SQLException.h
 * @file
 */


#define T ResultSet_T
typedef struct ResultSet_S *T;


//<< Protected methods

/**
 * Create a new ResultSet.
 * @param D the delegate used by this ResultSet
 * @param op delegate operations
 * @return A new ResultSet object
 */
T ResultSet_new(ResultSetDelegate_T D, Rop_T op);


/**
 * Destroy a ResultSet and release allocated resources.
 * @param R A ResultSet object reference
 */
void ResultSet_free(T *R);

//>> End Protected methods

/** @name Properties */
//@{

/**
 * Returns the number of columns in this ResultSet object.
 * @param R A ResultSet object
 * @return The number of columns
 */
int ResultSet_getColumnCount(T R);


/**
 * Get the designated column's name.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return Column name or NULL if the column does not exist. You 
 * should use the method ResultSet_getColumnCount() to test for 
 * the availablity of columns in the result set.
 */
const char *ResultSet_getColumnName(T R, int columnIndex);


/**
 * Returns column size in bytes. If the column is a blob then 
 * this method returns the number of bytes in that blob. No type 
 * conversions occur. If the result is a string (or a number 
 * since a number can be converted into a string) then return the 
 * number of bytes in the resulting string. 
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return Column data size
 * @exception SQLException if columnIndex is outside the valid range
 * @see SQLException.h
 */
long ResultSet_getColumnSize(T R, int columnIndex);

//@}

/**
 * Moves the cursor down one row from its current position. A
 * ResultSet cursor is initially positioned before the first row; the
 * first call to this method makes the first row the current row; the
 * second call makes the second row the current row, and so on. When
 * there are not more available rows false is returned. An empty
 * ResultSet will return false on the first call to ResultSet_next().
 * @param R A ResultSet object
 * @return true if the new current row is valid; false if there are no
 * more rows
 * @exception SQLException if a database access error occurs
 */
int ResultSet_next(T R);


/**
 * Returns true if the value of the designated column in the current row of
 * this ResultSet object is SQL NULL, otherwise false. If the column value is 
 * SQL NULL, a Result Set returns the NULL pointer for string and blob values
 * and 0 for primitive data types. Use this method if you need to differ 
 * between SQL NULL and the value NULL/0.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return True (1) if the column value is SQL NULL, otherwise false (0)
 * @exception SQLException if a database access error occurs or
 * columnIndex is outside the valid range
 * @see SQLException.h
 */
int ResultSet_isnull(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a C-string. If <code>columnIndex</code>
 * is outside the range [1..ResultSet_getColumnCount()] this
 * method throws an SQLException. <i>The returned string may only be 
 * valid until the next call to ResultSet_next() and if you plan to use
 * the returned value longer, you must make a copy.</i>
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return The column value; if the value is SQL NULL, the value
 * returned is NULL
 * @exception SQLException if a database access error occurs or 
 * columnIndex is outside the valid range
 * @see SQLException.h
 */
const char *ResultSet_getString(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a C-string. If <code>columnName</code>
 * is not found this method throws an SQLException. <i>The returned string
 * may only be valid until the next call to ResultSet_next() and if you plan
 * to use the returned value longer, you must make a copy.</i>
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return The column value; if the value is SQL NULL, the value
 * returned is NULL
 * @exception SQLException if a database access error occurs or 
 * columnName does not exist
 * @see SQLException.h
 */
const char *ResultSet_getStringByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as an int. If <code>columnIndex</code>
 * is outside the range [1..ResultSet_getColumnCount()] this
 * method throws an SQLException.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the value is NaN
 * @see SQLException.h
 */
int ResultSet_getInt(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as an int. If <code>columnName</code> is not 
 * found this method throws an SQLException.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0
 * @exception SQLException if a database access error occurs, columnName
 * does not exist or if the value is NaN
 * @see SQLException.h
 */
int ResultSet_getIntByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a long long. If <code>columnIndex</code>
 * is outside the range [1..ResultSet_getColumnCount()] this
 * method throws an SQLException.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0
 * @exception SQLException if a database access error occurs, 
 * columnIndex is outside the valid range or if the value is NaN
 * @see SQLException.h
 */
long long int ResultSet_getLLong(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a long long. If <code>columnName</code>
 * is not found this method throws an SQLException.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0
 * @exception SQLException if a database access error occurs, columnName
 * does not exist or if the value is NaN
 * @see SQLException.h
 */
long long int ResultSet_getLLongByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a double. If <code>columnIndex</code>
 * is outside the range [1..ResultSet_getColumnCount()] this
 * method throws an SQLException.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0.0
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the value is NaN
 * @see SQLException.h
 */
double ResultSet_getDouble(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a double. If <code>columnName</code> is 
 * not found this method throws an SQLException.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return The column value; if the value is SQL NULL, the value
 * returned is 0.0
 * @exception SQLException if a database access error occurs, columnName
 * does not exist or if the value is NaN
 * @see SQLException.h
 */
double ResultSet_getDoubleByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a void pointer. If <code>columnIndex</code>
 * is outside the range [1..ResultSet_getColumnCount()] this method 
 * throws an SQLException. <i>The returned blob may only be valid until
 * the next call to ResultSet_next() and if you plan to use the returned
 * value longer, you must make a copy.</i> 
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @param size The number of bytes in the blob is stored in size 
 * @return The column value; if the value is SQL NULL, the value
 * returned is NULL
 * @exception SQLException if a database access error occurs or 
 * columnIndex is outside the valid range
 * @see SQLException.h
 */
const void *ResultSet_getBlob(T R, int columnIndex, int *size);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a void pointer. If <code>columnName</code>
 * is not found this method throws an SQLException. <i>The returned
 * blob may only be valid until the next call to ResultSet_next() and if 
 * you plan to use the returned value longer, you must make a copy.</i>
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @param size The number of bytes in the blob is stored in size 
 * @return The column value; if the value is SQL NULL, the value
 * returned is NULL
 * @exception SQLException if a database access error occurs or 
 * columnName does not exist
 * @see SQLException.h
 */
const void *ResultSet_getBlobByName(T R, const char *columnName, int *size);


/** @name Date and Time  */
//@{

/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a Unix timestamp in the <i>local</i> timezone.
 * If <code>columnIndex</code> is outside the range 
 * [1..ResultSet_getColumnCount()] this method throws an SQLException. 
 * The returned value represent seconds since the <strong>epoch</strong>
 * (January 1, 1970, 00:00:00 GMT) in the <i>local</i> timezone. A SQL 
 * database will normally store timestamp values in UTC and on retrieval 
 * convert the value to the local timezone. If the SQL value contains a 
 * timezone part, this is respected when converting to Unix timestamp. 
 * Even though the underlying database might support timestamp ranges 
 * before the epoch and after '2038-01-19 03:14:07 UTC' it is safest not
 * to assume or use values outside this range. Especially if you are on 
 * a 32-bits system. In general and if timestamp with timezone support 
 * is not needed, it is more efficient to use an <code>integer</code> 
 * SQL data type in the database and Unix timestamp values directly.
 * This method can be used to retrieve the value of columns with the SQL
 * data type, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return The column value as seconds since the epoch in the local
 * timezone; if the value is SQL NULL, the value returned is 0, i.e. 
 * '1970-01-01 00:00:00 GMT'
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid timestamp
 * @see SQLException.h
 */
time_t ResultSet_getTimestamp(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of
 * this ResultSet object as a Unix timestamp in the <i>local</i> timezone. 
 * If <code>columnName</code> is not found this method throws an 
 * SQLException. The returned value represent seconds since the <strong>
 * epoch</strong> (January 1, 1970, 00:00:00 GMT) in the <i>local</i> 
 * timezone. A SQL database will normally store timestamp values in UTC 
 * and on retrieval convert the value to the local timezone. If the SQL 
 * value contains a timezone part, this is respected when converting to 
 * Unix timestamp. Even though the underlying database might support 
 * timestamp ranges before the epoch and after '2038-01-19 03:14:07 UTC'
 * it is safest not to assume or use values outside this range. Especially
 * if you are on a 32-bits system. In general and if timestamp with 
 * timezone support is not needed, it is more efficient to use an 
 * <code>integer</code> SQL data type in the database and Unix timestamp
 * values directly. This method can be used to retrieve the value of 
 * columns with the SQL data type, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return The column value as seconds since the epoch in the local
 * timezone; if the value is SQL NULL, the value returned is 0, i.e.
 * '1970-01-01 00:00:00 GMT'
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid timestamp
 * @see SQLException.h
 */
time_t ResultSet_getTimestampByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL Date object in the local timezone. If <code>
 * columnIndex</code> is outside the range [1..ResultSet_getColumnCount()]
 * this method throws an SQLException. This method can be used to retrieve 
 * the value of columns with the SQL data type, Date, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return A Date structure with fields for year, month and day (of month).
 * If the value is SQL NULL, an empty sqldate_t structure is returned
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL Date
 * @see SQLException.h
 */
sqldate_t ResultSet_getDate(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL Date object in the local timezone.  If 
 * <code>columnName</code> is not found this method throws an SQLException. 
 * This method can be used to retrieve the value of columns with the SQL 
 * data type, Date, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return A Date structure with fields for year, month and day (of month).
 * If the value is SQL NULL, an empty sqldate_t structure is returned
* @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL Date
 * @see SQLException.h
 */
sqldate_t ResultSet_getDateByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL Time object in the local timezone. If <code>
 * columnIndex</code> is outside the range [1..ResultSet_getColumnCount()]
 * this method throws an SQLException. This method can be used to retrieve
 * the value of columns with the SQL data type, Time, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return A Time structure with fields for hour, minute, seconds and
 * microseconds (if available). If the value is SQL NULL, an empty 
 * sqltime_t structure is returned
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL Time
 * @see SQLException.h
 */
sqltime_t ResultSet_getTime(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL Time object in the local timezone.  If
 * <code>columnName</code> is not found this method throws an SQLException.
 * This method can be used to retrieve the value of columns with the SQL
 * data type, Time, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return A Time structure with fields for hour, minute, seconds and
 * microseconds (if available). If the value is SQL NULL, an empty 
 * sqltime_t structure is returned
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL Time
 * @see SQLException.h
 */
sqltime_t ResultSet_getTimeByName(T R, const char *columnName);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL DateTime object in the local timezone. If <code>
 * columnIndex</code> is outside the range [1..ResultSet_getColumnCount()]
 * this method throws an SQLException. This method can be used to retrieve
 * the value of columns with the SQL data type, Date, Time, DateTime or 
 * Timestamp.
 * @param R A ResultSet object
 * @param columnIndex The first column is 1, the second is 2, ...
 * @return A DateTime structure with fields for date and time. If the value 
 * is SQL NULL, an empty sqldatetime_t structure is returned
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL DateTime
 * @see SQLException.h
 */
sqldatetime_t ResultSet_getDateTime(T R, int columnIndex);


/**
 * Retrieves the value of the designated column in the current row of this
 * ResultSet object as a SQL Time object in the local timezone.  If
 * <code>columnName</code> is not found this method throws an SQLException.
 * This method can be used to retrieve the value of columns with the SQL
 * data type, Time, DateTime or Timestamp.
 * @param R A ResultSet object
 * @param columnName The SQL name of the column. <i>case-sensitive</i>
 * @return A DateTime structure with fields for date and time. If the value 
 * is SQL NULL, an empty sqldatetime_t structure is returned
 * @exception SQLException if a database access error occurs, columnIndex
 * is outside the valid range or if the column value cannot be converted
 * to a valid SQL DateTime
 * @see SQLException.h
 */
sqldatetime_t ResultSet_getDateTimeByName(T R, const char *columnName);

//@}

#undef T
#endif

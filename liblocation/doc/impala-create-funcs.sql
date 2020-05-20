-- ============================================================================
-- The instructions below will create and register UDF's in the Impala metastore.
--
-- Important:
--
--  1) Remember that in the version of Apache Impala installed on
--     Cloudera Quickstart 5.13 there is no specific instruction to "re-create"
--     a UDF, it being necessary to delete it before it is created again.
--
--  2) In the "create function" instruction, the "location" parameter can be
--     changed, if necessary, if you copied the shared library to another
--     location.
--
--  3) Although it is possible to give other names to UDF's, we strongly advise
--     you to keep the names chosen by us. We ask this to maintain the
--     relationships between well-defined functions.
--
-- ============================================================================

-- Inform your database name or use the 'default'
-- CREATE DATABASE your-database-name;
USE default;

-- drop function geo_distance(double,double,double,double,string);
create function geo_distance(double,double,double,double,string) returns double
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_distance';

-- drop function geo_azimuth(double,double,double,double);
create function geo_azimuth(double,double,double,double) returns double
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_azimuth';

-- drop function geo_getutmvalues(string,string);
create function geo_getutmvalues(string,string) returns double
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_getutmvalues';

-- drop function geo_latlon_to_utm(double,double,int);
create function geo_latlon_to_utm(double,double,int) returns string
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_latlon_to_utm';

-- drop function geo_utm_to_latlon(double,double,boolean,int);
create function geo_utm_to_latlon(double,double,boolean,int) returns string
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_utm_to_latlon';

-- drop function geo_isvalidlat(double);
create function geo_isvalidlat(double) returns boolean
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_isvalidlat';

-- drop function geo_isvalidlon(double);
create function geo_isvalidlon(double) returns boolean
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_g_isvalidlon';

-- drop function unit_convert(string,double);
create function unit_convert(string,double) returns double
location '/user/cloudera/impala_udf/liblocation.so' SYMBOL='udf_unit_convert';

show functions;

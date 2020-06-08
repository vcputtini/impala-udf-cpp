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

-- drop function squid_logparts(string, string, string);
create function squid_logparts(string, string, string) returns string
location '/user/cloudera/impala_udf/liblogsquid.so' SYMBOL='udf_logparts';

-- drop function squid_totalsize_req(string,string);
create function squid_totalsize_req(string,string) returns bigint
location '/user/cloudera/impala_udf/liblogsquid.so' SYMBOL='udf_totalsizereq';


show functions;

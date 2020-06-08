## Native C++ UDF's for Apache Impala

Our goal is to create and make available native C++ UDFs for use
on Apache Impala.
Thereby helping to spread this wonderful feature that allows us to solve
problems of all kinds with performance and security.

As much as possible we will provide UDFs for use and learning.

We use **Cloudera Quickstart** for all our studies on the Hadoop ecosystem. With a special focus on Impala, HBASE and Flume.

- liblocation:<br>
It is a minimal library of functions related to Geo localization to be used directly in SQL statements in Apache Impala. See liblocation/README.md for details.

- liblogsquid:<br>
Provides UDFs to directly handle Squid's log line.
At the moment just handles built-in logformats.

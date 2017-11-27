DROP TABLE IF EXISTS `log`;
CREATE TABLE IF NOT EXISTS `log` (
`log_id` int(10) unsigned NOT NULL,
  `ts` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `raw` char(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `data`;
CREATE TABLE IF NOT EXISTS `data` (
`data_id` int(10) unsigned NOT NULL,
  `token` int(10) unsigned NOT NULL,
  `ts_insert` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ts_send` datetime NOT NULL,
  `sound` int(11) DEFAULT NULL,
  `light` int(11) DEFAULT NULL,
  `uv` int(11) DEFAULT NULL,
  `dust` int(11) DEFAULT NULL,
  `humidity` int(11) DEFAULT NULL,
  `temperature` int(11) DEFAULT NULL,
  `position` point DEFAULT NULL,
  `altitude` int(11) DEFAULT NULL,
  `z_acc` int(11) DEFAULT NULL,
  `pressure` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;